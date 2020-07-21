#define NUM_TIMES 4
#define NUM_BITS 128
#define HEX_BITS_LENGTH 4

// uncomment to turn on debugging
#define DEBUG

volatile unsigned long times[NUM_TIMES];
volatile int output_buffer[NUM_BITS/HEX_BITS_LENGTH];

volatile int output_counter = 0;
volatile int output_counter_hex = 0;
volatile int counter = 0;
volatile unsigned long start_time = 0;
volatile unsigned long end_time = 0;

volatile bool led_flip_flag = false;
bool flip_flag = true;

uint8_t ledPin = A5;

void interrupt_handler() {
  #ifdef DEBUG
  Serial.print(".");
  #endif
  led_flip_flag = !led_flip_flag;
  digitalWrite(ledPin, led_flip_flag);

  if (counter >= NUM_TIMES) {
    return;
  }

  unsigned long now = millis();

  if (now > times[counter]) {
    times[counter] = now;
  } else {
    counter = 0;
    initialize();
    return;
  }

  counter++;

}

void initialize() {
  output_counter = 0;
  output_counter_hex = 0;
  start_time = millis();
  // initialize with 0
  for (int i = 0; i < NUM_TIMES; i++) {
    times[i] = 0;
  }

  for (int i = 0; i < NUM_BITS / HEX_BITS_LENGTH; i++) {
    output_buffer[i] = 0;
  }
}

void print_results() {
  Serial.println("");
  Serial.println("{");
  Serial.print("\"hex\" : \"");
  for (int i = 0; i < NUM_BITS / HEX_BITS_LENGTH; i++) {
    Serial.print(output_buffer[i], HEX);
  }
  Serial.println("\",");
  
  Serial.print("\"bin\": \"");
  for (int i = 0; i < NUM_BITS / HEX_BITS_LENGTH; i++) {
    // print(number, BIN) doesn't print leading 0s
    for (unsigned int mask = 0x8; mask; mask >>= 1) {
      Serial.print(mask&output_buffer[i] ? '1' : '0');
    }
  }
  Serial.println("\",");
  Serial.print("\"total_time_ms\" : ");
  Serial.print(end_time - start_time);
  Serial.println();
  
  Serial.println("},");
}
void setup() {
  Serial.begin(9600);

  pinMode(2, INPUT);
  initialize();

  attachInterrupt(0, interrupt_handler, FALLING);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}

void loop() {

  if (NUM_TIMES == counter) {
    noInterrupts();
    long elapsed1, elapsed2 = 0;
    if (flip_flag) {
      elapsed1 = times[1] - times[0];
      elapsed2 = times[3] - times[2];
    } else {
      elapsed2 = times[1] - times[0];
      elapsed1 = times[3] - times[2];
    }

    if (elapsed1 != elapsed2) {

      output_buffer[output_counter_hex] = output_buffer[output_counter_hex] << 1;
      if (elapsed1 > elapsed2) {
        #ifdef DEBUG
        Serial.print("0");
        #endif
      } else {
        #ifdef DEBUG
        Serial.print("1");
        #endif

        output_buffer[output_counter_hex] = output_buffer[output_counter_hex] + 1;
      }
      
      output_counter = output_counter + 1;
      output_counter_hex = output_counter / HEX_BITS_LENGTH;
    }

    if (output_counter >= NUM_BITS) {
      end_time = millis();
      print_results();
      initialize();
    }
    
    flip_flag = !flip_flag;
    counter = 0;
    interrupts();
  }

}
