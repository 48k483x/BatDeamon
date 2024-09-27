# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <time.h>
# include <libnotify/notify.h>

int totalMins = 0;
char totalM[1000000];
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
    fclose(fp);
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
    char title[100];

    snprintf(details, sizeof(details), "Your Battery Now Is > %s%%", batCap); 
    snprintf(title, sizeof(title), "After > %s Min", notif);

    notify_init("Battery Monitor");
    NotifyNotification * Hello = notify_notification_new(title, details, "dialog information");
    notify_notification_set_timeout(Hello, 5000);
    if (!(notify_notification_show (Hello, NULL))) fprintf(stderr, "failed to push notification");
    
    g_object_unref(G_OBJECT(Hello));
    notify_uninit();
}

int main ( void )
{
    int BatCapacity;
    char battery[4];
    char timeChar[10];
    int LoopBattery;
    char *time_str;
    time_t start, current;
    double diff;

    start = time(NULL);

    BatCapacity = getBatteryCapacity();
    time_str = getCurrentTime();
    snprintf(battery, sizeof(battery), "%d", BatCapacity);
    snprintf(timeChar, sizeof(timeChar), "%ld", start);
    printf("Program started at : %s\n", time_str);
    setNotif(timeChar, battery);

    while (1337)
    {
        LoopBattery = getBatteryCapacity();
        if (LoopBattery == (BatCapacity - 1))
        {
            current = time(NULL);
            diff = difftime(current, start);
            diff /= 60;
            totalMins += diff;
            BatCapacity = LoopBattery;
            snprintf(totalM, sizeof(totalM), "%.2f", diff);
            snprintf(battery, sizeof(battery), "%d", BatCapacity);
            setNotif(totalM, battery);
        }
        else continue ;

    }
}
