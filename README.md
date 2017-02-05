# davhttp
minimal dependency C code for HTTP GET and POST (on Windows)

IMPORTANT: only GET is implemented.

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

INSTALL MINGW and COMPILE with
gcc davhttp.c -o davhttp.exe -lws2_32


This code is public domain.

