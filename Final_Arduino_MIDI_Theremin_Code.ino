/*
  Ultrasonic --> MIDI Theremin (8 fixed notes, smoothed with hysteresis)
  - 5 to 50 cm split into 8 zones (~5.6 cm each)
  - Distance is averaged to reduce jitter
  - Hysteresis added to prevent rapid flickering
*/

const int trigPin = 9;
const int echoPin = 10;

const int minDistance = 5;
const int maxDistance = 50;
const int midiChannel = 1;

long durationUs;
int distanceCm;
int currentNote = -1;
int currentZone = -1; // track active zone

// Our chosen 8 notes (MIDI numbers)
const int notes[8] = {48, 49, 52, 53, 55, 56, 59, 60};
// C3, C#3, E3, F3, G3, G#3, B3, C4

// --- Helper: take N samples and average ---
int readDistanceCm() {
  const int samples = 5;
  long sum = 0;
  int valid = 0;

  for (int i = 0; i < samples; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long d = pulseIn(echoPin, HIGH, 30000); // timeout 30ms
    if (d > 0) {
      sum += (long)(d * 0.0343 / 2.0 + 0.5);
      valid++;
    }
    delay(5); // small gap between samples
  }

  if (valid == 0) return -1;  // no echo at all
  return (int)(sum / valid);
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);  // for Hairless MIDI bridge
}

void loop() {
  distanceCm = readDistanceCm();

  if (distanceCm == -1 || distanceCm < minDistance || distanceCm > maxDistance) {
    // Out of range -> silence
    if (currentNote != -1) {
      sendNoteOff(midiChannel, currentNote, 0);
      currentNote = -1;
      currentZone = -1;
    }
    delay(20);
    return;
  }

  // Map distance to one of 8 zones
  int zoneSize = (maxDistance - minDistance) / 8;  // ≈5.6 cm
  int newZone = (distanceCm - minDistance) / zoneSize;
  if (newZone > 7) newZone = 7;

  // Hysteresis margin in cm
  const int margin = 2;

  if (currentZone == -1) {
    // First time entering
    currentZone = newZone;
    currentNote = notes[currentZone];
    sendNoteOn(midiChannel, currentNote, 100);
  } 
  else if (newZone != currentZone) {
    // Only switch if distance is clearly past the margin
    int boundaryLow = minDistance + currentZone * zoneSize;
    int boundaryHigh = boundaryLow + zoneSize;

    if ((newZone > currentZone && distanceCm > boundaryHigh + margin) ||
        (newZone < currentZone && distanceCm < boundaryLow - margin)) {
      sendNoteOff(midiChannel, currentNote, 0);
      delay(2);
      currentZone = newZone;
      currentNote = notes[currentZone];
      sendNoteOn(midiChannel, currentNote, 100);
    }
  }

  delay(20);
}

// ---------------- MIDI helper functions ----------------
void sendNoteOn(byte channel, byte pitch, byte velocity) {
  Serial.write(0x90 | ((channel - 1) & 0x0F));
  Serial.write(pitch & 0x7F);
  Serial.write(velocity & 0x7F);
}

void sendNoteOff(byte channel, byte pitch, byte velocity) {
  Serial.write(0x80 | ((channel - 1) & 0x0F));
  Serial.write(pitch & 0x7F);
  Serial.write(velocity & 0x7F);
}
