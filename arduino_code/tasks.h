QueueHandle_t xQueue;
QueueHandle_t xQueueLCD;

static void vReadDisplayTemp( void *pvParameters );
static void vReadDisplayPot( void *pvParameters );
static void vMangageHeater( void *pvParameters );
static void vManageLum( void *pvParameters );
static void vLcdControl( void *pvParameters );
