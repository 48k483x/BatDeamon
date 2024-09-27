# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <time.h>
# include <libnotify/notify.h>

int totalMins = 0;
int totalHours = 0;

int getBatteryCapacity()
{
    FILE *fp;
    char buf[64];
    int capacity = -1;

    fp = fopen("/sys/class/power_supply/BAT1/capacity", "r");
    if (fp == NULL) return (fprintf(stderr, "fopen() returned NULL"));
    
    if (fgets(buf, sizeof(buf), fp) != NULL) capacity = atoi(buf);
    else perror("fgets can't read the next char!");
    return capacity;
}

char *getCurrentTime()
{
    time_t starTime;
    struct tm *timeInfo;
    char *time_str = malloc(20);
    if (time_str == NULL) return NULL;

    starTime = time(NULL);
    timeInfo = localtime(&starTime);
    strftime(time_str, 20, "%Y-%m-%d %H:%M:%S", timeInfo);
    return time_str;
}

void setNotif(char *notif, char *batCap)
{
    char details[100];

    snprintf(details, sizeof(details), "Your Battery Now Is > %s%%", batCap); 
    notify_init("Battery Monitor");
    NotifyNotification * Hello = notify_notification_new(notif, details, "dialog information");
    notify_notification_set_timeout(Hello, 5000);
    if (!(notify_notification_show (Hello, NULL))) fprintf(stderr, "failed to push notification");
    
    g_object_unref(G_OBJECT(Hello));
    notify_uninit();
}

int main ( void )
{
    int BatCapacity;
    char battery[4];
    int LoopBattery;
    char *time_str;
    time_t start, current;
    double diff;

    start = time(NULL);

    BatCapacity = getBatteryCapacity();
    time_str = getCurrentTime();
    snprintf(battery, sizeof(battery), "%d", BatCapacity);
    setNotif("Battery Status", battery);

    while (1337)
    {
        LoopBattery = getBatteryCapacity();
        if (LoopBattery == (BatCapacity - 10))
        {
            current = time(NULL);
            diff = difftime(current, start);
            diff /= 60;
            totalMins += diff;
            BatCapacity = LoopBattery;

        }
    }
}
