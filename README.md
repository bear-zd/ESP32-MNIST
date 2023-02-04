# ESP32-MNIST

## hardware

hardware code developed based on platformIO (a vscode extansion). Just open the hardware folder as project and adjust some code to satisfy you device and build/upload.

spend of memory:

> Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
> RAM:   [==        ]  23.8% (used 77852 bytes from 327680 bytes)
> Flash: [========= ]  87.5% (used 1147233 bytes from 1310720 bytes)

## train and process

the model was trained on my computer cpu based on tensorflow-cpu. and the data was processed and upload to the server. Also build a server to send data to my ESP32 to get data.

the python dependency mainly use: tensorflow-cpu==2.11.0 numpy tqdm

## more details

[zidea's blog about cs249r](https://blog.zidea.site/p/cs249r-%E5%B5%8C%E5%85%A5%E5%BC%8F%E6%9C%BA%E5%99%A8%E5%AD%A6%E4%B9%A0/#mnist-example)

[cs249r- tinyML](https://sites.google.com/g.harvard.edu/tinyml/home) 