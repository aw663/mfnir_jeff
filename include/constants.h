#define uBAT_LOW false
#define uBAT_OK true
#define uBAT_LOW_THRESHOLD  3.2
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 3 // in seconds
#define formatString  "D %i,%i,%i,%i,%i,%.2f,%.2f"

bool uPOWER_STATE = uBAT_OK;

char bufferSerial[200];
const String devID = "mfNIR_SS2";
const int samples = 256; //Samples for FFT. This value MUST ALWAYS be a power of 2
const int frameSize = 1000;
const int samplingRate = 1000;
const int res = 4096;

int onDelay = 10; //set as 10 ms since last 2 ms is for ADC. 200 for the original mfnir
int intensity[4] = {128,128,128,128};
int dly_avg = 2; //in us
int n_reads = 50;

int samplingFrequency = 1000;
int incomingByte;   // for incoming Serial data
String inData;

int freq = 150000; //OG had 5000
int channel[4] = {0,1,2,3};
int resolution = 8;

bool cmdOK = false;
long loopStart;

int state = 0;

int valCH1=0;
int valCH2=0;
int valCH3=0;
int valCH4=0;
int valCH5=0;
int valCH6=0;

int darkCH1=0;
int darkCH2=0;
int darkCH3=0;
int darkCH4=0;

int onCH1=0;
int onCH2=0;
int onCH3=0;
int onCH4=0;
float vBat = 5.00;
const char* serverIndex = "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
"<input type='file' name='update'>"
"<input type='submit' value='Update'>"
"</form>"
"<div id='prg'>progressoo: 0%</div>"
"<script>"
"$('form').submit(function(e){"
"e.preventDefault();"
"var form = $('#upload_form')[0];"
"var data = new FormData(form);"
" $.ajax({"
"url: '/update',"
"type: 'POST',"
"data: data,"
"contentType: false,"
"processData:false,"
"xhr: function() {"
"var xhr = new window.XMLHttpRequest();"
"xhr.upload.addEventListener('progress', function(evt) {"
"if (evt.lengthComputable) {"
"var per = evt.loaded / evt.total;"
"$('#prg').html('progress: ' + Math.round(per*100) + '%');"
"}"
"}, false);"
"return xhr;"
"},"
"success:function(d, s) {"
"console.log('success!')"
"},"
"error: function (a, b, c) {"
"}"
"});"
"});"
"</script>";

const char* host = "esp32webupdate";
char* ssid = "mfnir";
char* password = "password";
