# davhttp
minimal dependency C code for HTTP GET and POST (only Windows so far)

### What it does (not) 
Almost nothing. It can only GET small html and plain text files.
It does not manage chunked responses. Thus it requests a HTTP/1.0

    void main(int argc, char *argv[])
    {
       char headers[64*1024];
       char body[64*1024];
    
       struct Response resp={ .headers=headers, .body=body};
       GET("www.wikipedia.org",80,"/",&resp);
       puts("== headers ==");
       puts(resp.headers);
    
       puts("== body ==");
       puts(resp.body);
    
    }

### Getting started
Install MINGW and compile all with

    gcc davhttp.c -o davhttp.exe -lws2_32

### TODO 
make robust and then implement POST
chunked encoding, maybe

### License
This code is public domain.

