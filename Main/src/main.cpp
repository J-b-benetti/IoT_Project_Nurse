// Include Libraries
#include <esp_now.h>
#include <WiFi.h>

// Define a data structure
typedef struct struct_message
{
  char id[32];
  bool ide1;
  bool ide2;
  bool ide3;
  bool sonnette4;
} struct_message;

// Create a structured object
struct_message myDataReceveid;
struct_message salle_de_soin_my_data{
    "Salle de Soin",
    false,
    false,
    false,
    false};
uint8_t adresseRecepteur[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Adresse MAC du récepteur

// Callback function executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&myDataReceveid, incomingData, sizeof(myDataReceveid));
  Serial.println();
  Serial.println("RECEPTION DE SALLE DE SOIN : ");
  Serial.println("FROM : ");
  Serial.println(myDataReceveid.id);
  Serial.println();
  if (myDataReceveid.ide1)
  {
    Serial.print("IDE 1 est là");
    salle_de_soin_my_data.ide1 = false;
  }
  else
  {
    Serial.print("IDE 1 absente");
  }
  Serial.println();
  if (myDataReceveid.ide2)
  {
    Serial.print("IDE 2 est là");
    salle_de_soin_my_data.ide2 = false;
  }
  else
  {
    Serial.print("IDE 2 absente");
  }

  Serial.println();
  if (myDataReceveid.ide3)
  {
    Serial.print("IDE 3 est là");
    salle_de_soin_my_data.ide3 = false;
  }
  else
  {
    Serial.print("IDE 3 absente");
  }
  Serial.println();
  if (myDataReceveid.sonnette4)
  {
    Serial.print("Sonnette +");
  }
  else
  {
    Serial.print("Sonnette -");
  }
  Serial.println();
}

#pragma region ButtonLogic
// Constantes pour les broches des boutons
#define BOUTON_1_PIN D2 // Broche GPIO à laquelle est connecté le premier bouton
#define BOUTON_2_PIN D3 // Broche GPIO à laquelle est connecté le deuxième bouton
#define BOUTON_3_PIN D4 // Broche GPIO à laquelle est connecté le troisième bouton
#define BOUTON_4_PIN D5 // Broche GPIO à laquelle est connecté le quatrième bouton
#pragma endregion

char macStr[18];

void envoyerDonnees()
{
  if (digitalRead(BOUTON_1_PIN) == HIGH)
  {
    salle_de_soin_my_data.ide1 = true;
    Serial.println(salle_de_soin_my_data.id);
    Serial.println("a envoyé à");
    Serial.println(macStr);
  }
  else if (digitalRead(BOUTON_2_PIN) == HIGH)
  {
    salle_de_soin_my_data.ide2 = true;
    Serial.println(salle_de_soin_my_data.id);
    Serial.println("a envoyé à");
    Serial.println(macStr);
  }
  else if (digitalRead(BOUTON_3_PIN) == HIGH)
  {
    salle_de_soin_my_data.ide3 = true;
    Serial.println(salle_de_soin_my_data.id);
    Serial.println("a envoyé à");
    Serial.println(macStr);
  }

  else if (digitalRead(BOUTON_4_PIN) == HIGH)
  {
    salle_de_soin_my_data.sonnette4 = true;
    Serial.println(salle_de_soin_my_data.id);
    Serial.println("a envoyé à");
    Serial.println(macStr);
  }
  void broadcast();
  broadcast();
}

void broadcast()
// Emulates a broadcast
{
  // Broadcast a message to every device in range
  uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  esp_now_peer_info_t peerInfo = {};
  memcpy(&peerInfo.peer_addr, broadcastAddress, 6);
  if (!esp_now_is_peer_exist(broadcastAddress))
  {
    esp_now_add_peer(&peerInfo);
  }
  // Send message
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&salle_de_soin_my_data, sizeof(salle_de_soin_my_data));

#pragma region Gestion Erreur
  // Print results to serial monitor
  if (result == ESP_OK)
  {
    Serial.println("Broadcast message success");
  }
  else if (result == ESP_ERR_ESPNOW_NOT_INIT)
  {
    Serial.println("ESP-NOW not Init.");
  }
  else if (result == ESP_ERR_ESPNOW_ARG)
  {
    Serial.println("Invalid Argument");
  }
  else if (result == ESP_ERR_ESPNOW_INTERNAL)
  {
    Serial.println("Internal Error");
  }
  else if (result == ESP_ERR_ESPNOW_NO_MEM)
  {
    Serial.println("ESP_ERR_ESPNOW_NO_MEM");
  }
  else if (result == ESP_ERR_ESPNOW_NOT_FOUND)
  {
    Serial.println("Peer not found.");
  }
  else
  {
    Serial.println("Unknown error");
  }
#pragma endregion
}

void setup()
{
  // Set up Serial Monitor
  Serial.begin(115200);

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  pinMode(BOUTON_1_PIN, INPUT_PULLUP);
  pinMode(BOUTON_2_PIN, INPUT_PULLUP);
  pinMode(BOUTON_3_PIN, INPUT_PULLUP);
  pinMode(BOUTON_4_PIN, INPUT_PULLUP);

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
  // Envoyer les données si un bouton est cliqué
  if (digitalRead(BOUTON_1_PIN) == HIGH || digitalRead(BOUTON_2_PIN) == HIGH || digitalRead(BOUTON_3_PIN) == HIGH || digitalRead(BOUTON_4_PIN) == HIGH)
  {
    envoyerDonnees();
  }

  // Rien à faire dans la boucle loop() car tout se passe dans les fonctions de rappel
  delay(100);
}