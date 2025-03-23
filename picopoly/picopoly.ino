#include <WiFi.h>

char ssid[] = "Pico-Monopoly";
char password[] = "play1234";

WiFiServer server(80);

struct Player {
  char name[20];
  int money;
  int position;
  int properties[12];
  int propertyCount;
  int income;
  int spending;
};

struct BoardSpace {
  char name[20];
  char type[10];
  int cost;
  int rent;
  int value;
};

// Updated player names
Player players[2] = {{"Salma", 1500, 0, {0}, 0, 0, 0}, {"Tabish", 1500, 0, {0}, 0, 0, 0}};
BoardSpace board[12] = {
  {"GO", "go", 0, 0, 200}, {"Mediterranean Ave", "property", 60, 2, 0},
  {"Community Chest", "chest", 0, 0, 0}, {"Baltic Ave", "property", 60, 4, 0},
  {"Income Tax", "tax", 0, 0, -200}, {"Reading Railroad", "utility", 200, 25, 0},
  {"Oriental Ave", "property", 100, 6, 0}, {"Chance", "chance", 0, 0, 0},
  {"Vermont Ave", "property", 100, 6, 0}, {"Free Parking", "free", 0, 0, 0},
  {"St. James Place", "property", 140, 10, 0}, {"Go to Jail", "jail", 0, 0, 0}
};

int currentPlayer = 0;
bool gameOver = false;
String lastAction = "Game started! Salma's turn.";

int rollDice() {
  return random(1, 7) + random(1, 7);
}

String handleSpace(Player &p, BoardSpace space, int dice) {
  if (strcmp(space.type, "go") == 0) {
    p.money += space.value;
    p.income += space.value;
    return "Collected $200 at GO!";
  } else if (strcmp(space.type, "property") == 0 || strcmp(space.type, "utility") == 0) {
    int owner = -1;
    for (int i = 0; i < 2; i++) {
      if (players[i].properties[p.position] == 1) owner = i;
    }
    if (owner == -1) {
      if (p.money >= space.cost) {
        p.money -= space.cost;
        p.spending += space.cost;
        p.properties[p.position] = 1;
        p.propertyCount++;
        return "Bought " + String(space.name) + " for $" + String(space.cost) + "!";
      }
      return "Not enough money!";
    } else if (owner != currentPlayer) {
      p.money -= space.rent;
      p.spending += space.rent;
      players[owner].money += space.rent;
      players[owner].income += space.rent;
      if (p.money < 0) {
        gameOver = true;
        return "Paid $" + String(space.rent) + " rent to " + String(players[owner].name) + ". Bankrupt! Game Over.";
      }
      return "Paid $" + String(space.rent) + " rent to " + String(players[owner].name) + ".";
    }
    return "You own this!";
  } else if (strcmp(space.type, "chance") == 0 || strcmp(space.type, "chest") == 0) {
    int amount = random(-100, 101);
    p.money += amount;
    if (amount > 0) p.income += amount;
    else p.spending += abs(amount);
    if (p.money < 0) {
      gameOver = true;
      return String(space.type) + ": " + (amount > 0 ? "Gained $" : "Lost $") + String(abs(amount)) + ". Bankrupt! Game Over.";
    }
    return String(space.type) + ": " + (amount > 0 ? "Gained $" : "Lost $") + String(abs(amount)) + ".";
  } else if (strcmp(space.type, "tax") == 0) {
    p.money += space.value;
    p.spending += abs(space.value);
    if (p.money < 0) {
      gameOver = true;
      return "Paid $200 tax. Bankrupt! Game Over.";
    }
    return "Paid $200 tax.";
  } else if (strcmp(space.type, "jail") == 0) {
    p.position = 4;
    return "Sent to Jail!";
  }
  return "Landed on " + String(space.name) + ".";
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  
  Serial.println("Starting AP...");
  WiFi.mode(WIFI_AP);
  if (WiFi.beginAP(ssid, password)) {
    Serial.println("AP started successfully!");
  } else {
    Serial.println("Failed to start AP!");
    while (true);
  }
  delay(2000);
  Serial.print("AP active: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected!");
    String request = client.readStringUntil('\r');
    Serial.println("Request: " + request);
    client.flush();

    if (request.indexOf("GET /roll") != -1) {
      int dice = rollDice();
      players[currentPlayer].position = (players[currentPlayer].position + dice) % 12;
      lastAction = "Rolled " + String(dice) + ". " + handleSpace(players[currentPlayer], board[players[currentPlayer].position], dice);
      if (!gameOver) currentPlayer = (currentPlayer + 1) % 2;
    }

    String html = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    html += "<!DOCTYPE html><html><head><title>Monopoly</title>";
    html += "<style>body { font-family: Arial; text-align: center; background: #f0f0f0; margin: 0; padding: 20px; }";
    html += "h1 { color: #2c3e50; } button { padding: 12px 24px; font-size: 18px; background: #3498db; color: white; border: none; border-radius: 5px; cursor: pointer; }";
    html += "button:disabled { background: #bdc3c7; } .board { display: grid; grid-template-columns: repeat(4, 1fr); gap: 5px; margin: 20px auto; width: 90%; max-width: 600px; }";
    html += ".board div { padding: 10px; border: 2px solid #333; border-radius: 5px; font-size: 14px; text-align: center; }";
    html += ".go { background: #2ecc71; color: white; } .property { background: #e74c3c; color: white; }";
    html += ".utility { background: #f1c40f; color: black; } .chance { background: #e67e22; color: white; }";
    html += ".chest { background: #9b59b6; color: white; } .tax { background: #7f8c8d; color: white; }";
    html += ".jail { background: #34495e; color: white; } .free { background: #ecf0f1; color: black; }";
    html += ".highlight { border: 3px solid #3498db; } .player { font-weight: bold; color: #e74c3c; }";
    html += ".stats { font-size: 14px; margin: 10px; background: #fff; padding: 10px; border-radius: 5px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }</style>";
    html += "</head><body><h1>Monopoly</h1>";
    html += "<p class='player'>" + String(players[currentPlayer].name) + "'s Turn | Money: $" + String(players[currentPlayer].money) + "</p>";
    html += "<p>Position: " + String(board[players[currentPlayer].position].name) + "</p>";
    html += "<p>" + lastAction + "</p>";
    html += "<div class='stats'>Salma: $" + String(players[0].money) + " (Income: $" + String(players[0].income) + ", Spending: $" + String(players[0].spending) + ")</div>";
    html += "<div class='stats'>Tabish: $" + String(players[1].money) + " (Income: $" + String(players[1].income) + ", Spending: $" + String(players[1].spending) + ")</div>";
    if (!gameOver) html += "<a href='/roll'><button>Roll Dice</button></a>";
    else html += "<button disabled>Game Over</button>";
    html += "<div class='board'>";
    for (int i = 0; i < 12; i++) {
      String className = String(board[i].type);
      html += "<div class='" + className + (players[0].position == i || players[1].position == i ? " highlight" : "") + "'>";
      html += String(board[i].name);
      if (players[0].properties[i] == 1) html += " (Salma)";
      else if (players[1].properties[i] == 1) html += " (Tabish)";
      html += "</div>";
    }
    html += "</div></body></html>";

    client.print(html);
    client.stop();
    Serial.println("Response sent");
  }
}