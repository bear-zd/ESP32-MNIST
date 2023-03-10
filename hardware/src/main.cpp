
#include <WiFi.h>

#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "MNISTModel.h"
#include "dataHandler.h"
#include <TFT_eSPI.h>
#include <SPI.h>

int R_in = 25;
int G_in = 33;
int B_in = 32;
namespace{
    tflite::ErrorReporter* error_reporter = nullptr;
    const tflite::Model* model = nullptr;
    tflite::MicroInterpreter* interpreter = nullptr;
    TfLiteTensor* model_input = nullptr;
    TFT_eSPI tft = TFT_eSPI();

    constexpr int kTensorArenaSize = 32 * 1024;
    uint8_t tensor_arena[kTensorArenaSize];
    
    const char* ssid = "Honor20";
    const char* password = "12345670";
}
#include <SPI.h> 
#include <TFT_eSPI.h>
void showImgage(int predictV, int trueV, int8_t* image){
    
    for(int i=0;i<784;i++){
        tft.fillRect((i%28)*3,(i/28)*3,3,3,image[i]);

    }
    tft.drawRect(0,0,28*3,28*3,TFT_BLACK);
    tft.setCursor(10, 90, 1);    // 设置起始坐标(10, 10)，2 号字体
    tft.setTextColor(TFT_BLACK); // 设置字体颜色
    tft.setTextSize(1);          // 设置文字的大小 (1~7)
    tft.printf("predict:%d",predictV);
    tft.setCursor(10, 100, 1);
    tft.printf("true:%d",trueV);  
}

void light(int R, int G, int B){
    analogWrite(R_in, R);
    analogWrite(G_in, G);
    analogWrite(B_in, B);
}

void setup() {
    Serial.begin(115200);
    while(!Serial);
    tft.init();

    pinMode(R_in, OUTPUT);
    pinMode(G_in, OUTPUT);
    pinMode(B_in, OUTPUT);
    Serial.println("Serial init successful");
    // start wifi connect!
    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
    // init tml
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;

    model = tflite::GetModel(MNIST_model_data);

    if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report(
        "Model provided is schema version %d not equal "
        "to supported version %d.",
        model->version(), TFLITE_SCHEMA_VERSION);
    return;
    }

    static tflite::MicroMutableOpResolver<4> micro_mutable_op_resolver;
    if(micro_mutable_op_resolver.AddConv2D()!=kTfLiteOk) error_reporter->Report("resolver not successfully setup!");
    if(micro_mutable_op_resolver.AddAveragePool2D()!=kTfLiteOk) error_reporter->Report("resolver not successfully setup!");
    if(micro_mutable_op_resolver.AddFullyConnected()!=kTfLiteOk) error_reporter->Report("resolver not successfully setup!");
    if(micro_mutable_op_resolver.AddReshape()!=kTfLiteOk) error_reporter->Report("resolver not successfully setup!");

    static tflite::MicroInterpreter static_interpreter(
        model, micro_mutable_op_resolver, tensor_arena, kTensorArenaSize, error_reporter
    );
    interpreter = &static_interpreter;

    if(interpreter->AllocateTensors()!=kTfLiteOk) error_reporter->Report("Allocate failed!");

    model_input = interpreter->input(0);

    Serial.println("setup successful!");
}

void loop() {
    int8_t label = getData(model_input->data.int8);
    int8_t image[784];
    for(int i=0;i<784;i++) image[i] = model_input->data.int8[i];
    if(label == -48 || label == -1)
        error_reporter->Report("failed to get vilid data!");
    // Run inference, and report any error
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
    error_reporter->Report("Invoke failed.\n");
    return;
    }
    int8_t* output = interpreter->output(0)->data.int8;
    // for(int i=0;i<10;i++)
    //     output[i] = (output[i] - 24) * 0.21172021329402924;
    int argmax=0;
    for(int j=0;j<10;j++)
    {
        if (output[j] > output[argmax])
            argmax = j;
    }
    Serial.printf("predicted label:%d\t true value:%d\n",argmax,label);
    showImgage(argmax,label, image);
    // if(argmax == label) light(0,255,0);
    // else light(255,0,0);
    delay(1000);
    tft.fillScreen(TFT_WHITE);
    // light(0,0,0);

}