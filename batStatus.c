# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <time.h>
# include <libnotify/notify.h>

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

void setNotif(char *notif, char *details)
{
    notify_init(notif);
    NotifyNotification * Hello = notify_notification_new(notif, details, "dialog info");
    notify_notification_show (Hello, NULL);
    g_object_unref(G_OBJECT(Hello));
    notify_uninit();
}

int main ( void )
{
    int BatCapacity;
    int LoopBattery;
    
    BatCapacity = getBatteryCapacity();
    //if (BatCapacity != 100)
        //return (fprintf(stderr, "Warning:Your Battery Capacity should == 100%.\nYour Battey == %d%\n", BatCapacity));
    char *time_str = getCurrentTime();
    printf("%s\n", time_str);
    setNotif(time_str, "Battery level decreased by 10%%");
/*    while (1337)
    {
        LoopBattery = getBatteryCapacity();
        if (BatCapacity == 100 && LoopBattery == 90)
        {
            printf();
        }
    }
*/
}
