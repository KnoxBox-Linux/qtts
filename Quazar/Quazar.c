#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *url = NULL;
    char *savefile = NULL;
    
    // URL input
    FILE *p = popen("zenity --entry --title='Quazar' --text='Enter URL:' --entry-text='https://example.com'", "r");
    if (p) {
        char buf[512];
        if (fgets(buf, sizeof(buf), p)) {
            url = strdup(buf); 
            url[strcspn(url, "\n")] = 0;
        }
        pclose(p);
    }
    
    if (!url || strlen(url) < 5) return 1;
    
    int is_download = system("zenity --question --title='Quazar' --text='Download or View?' --ok-label='Download' --cancel-label='View' --width=300") == 0;
    
    char buf[512];
    if (is_download) {
        
        p = popen("zenity --file-selection --save --filename='download.html'", "r");
        if (p) {
            if (fgets(buf, sizeof(buf), p)) {
                savefile = strdup(buf); 
                savefile[strcspn(savefile, "\n")] = 0;
            }
            pclose(p);
        }
    }
    
    char cmd[1024];
    if (is_download && savefile) {
        snprintf(cmd, sizeof(cmd), "curl -sL '%s' -o '%s' && zenity --info --title='Quazar' --text='Saved to %s!'", url, savefile, savefile);
    } else {
        snprintf(cmd, sizeof(cmd), "curl -sL '%s' | less", url);
    }
    
    printf("Loading %s...\n", url);
    if (system(cmd) == -1) {}
    
    free(url); 
    if (savefile) free(savefile);
    return 0;
}
