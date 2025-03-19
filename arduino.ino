#define SAMPLE_RATE 500
#define BUFFER_SIZE 128
#define motorPin 3
#define SEND_INTERVAL 50  // Number of samples between transmissions

int circular_buffer[BUFFER_SIZE];
int data_index, sum;
int sampleCounter = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Timestamp,Raw_EMG,Envelope"); // CSV Header
    pinMode(motorPin, OUTPUT);
}

void loop() {
    static unsigned long past = 0;
    unsigned long present = micros();
    unsigned long interval = present - past;
    past = present;

    static long timer = 0;
    timer -= interval;

    if (timer < 0) {
        timer += 1000000 / SAMPLE_RATE;
        int sensor_value = analogRead(A0);
        int signal = EMGFilter(sensor_value);
        int envelope = getEnvelope(abs(signal));

        sampleCounter++;
        // Only send data every SEND_INTERVAL samples
        if (sampleCounter >= SEND_INTERVAL) {
            if(envelope <= 5){
                digitalWrite(motorPin, LOW);
                Serial.print("0");
                Serial.print(", ");
                Serial.println(envelope);
            } else {
                digitalWrite(motorPin, HIGH);
                Serial.print("1");
                Serial.print(", ");
                Serial.println(envelope);
            }
            sampleCounter = 0;
        }
    }
}

// Envelope Detection (Moving Average)
int getEnvelope(int abs_emg) {
    sum -= circular_buffer[data_index];
    sum += abs_emg;
    circular_buffer[data_index] = abs_emg;
    data_index = (data_index + 1) % BUFFER_SIZE;
    return (sum / BUFFER_SIZE) * 2;
}

// Band-Pass Butterworth IIR Filter (20Hz–500Hz)
float EMGFilter(float input) {
    float output = input;
    {
        static float z1, z2;
        float x = output - 0.05159732 * z1 - 0.36347401 * z2;
        output = 0.01856301 * x + 0.03712602 * z1 + 0.01856301 * z2;
        z2 = z1;
        z1 = x;
    }
    {
        static float z1, z2;
        float x = output - (-0.53945795 * z1) - 0.39764934 * z2;
        output = 1.00000000 * x + (-2.00000000 * z1) + 1.00000000 * z2;
        z2 = z1;
        z1 = x;
    }
    {
        static float z1, z2;
        float x = output - 0.47319594 * z1 - 0.70744137 * z2;
        output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
        z2 = z1;
        z1 = x;
    }
    {
        static float z1, z2;
        float x = output - (-1.00211112 * z1) - 0.74520226 * z2;
        output = 1.00000000 * x + (-2.00000000 * z1) + 1.00000000 * z2;
        z2 = z1;
        z1 = x;
    }
    return output;
}
