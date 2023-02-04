#include "dataHandler.h"
#include <HTTPClient.h>
#include <sstream>
#include <string>
#include <malloc.h>

String serverPath = "http://zidea.site:5000/randM";

int8_t getData(signed char *input)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        http.begin(serverPath);
        int httpCode = http.GET();

        char *buf = (char *)malloc(4096 * sizeof(char));
        String s = http.getString();
        s.toCharArray(buf, 4096);

        std::stringstream ss(buf);
        for (int i = 0; i < 784; i++)
        {
            int t;
            ss >> t;
            input[i] = t;
        }
        int8_t label = 0;
        while(ss)
        ss >> label;

        return label-48;
    }
    else
    {
        return -1;
    }
}
