- 6 Giao thức TCP, UDP, ICMP, DNS, HTTP, HTTPS sử dụng socket
- client chạy trên window sử dụng Visual Studio 2017
- server chạy trên Ubuntu sử dụng terminal

Giao thức tự động gửi tin nhắn từ client -> server và ngược lại. kết thúc khi tin nhắn ("*")
Cách chạy:
  - Chạy quyền Admin để gửi ICMP
  - Trên window tải openssl-1.1.0f-vs2017 
  *) Config trên visual studio
    + Project -> Properties 
          - C/C++ -> general -> Additional Include Directories -> ..\openssl-1.1.0f-vs2017\include
          - Linker -> general -> Additional Include Directories -> ..\openssl-1.1.0f-vs2017\lib
          - Input -> thêm 2 mục lib vào Additional Dependencies (libsslMT.lib;libcryptoMT.lib;).
    
  - Trên Ubuntu:
    + Cài đặt thư viện SSL : sudo apt-get install libssl-dev
    + gcc -Wall -o server all_server.c -L/usr/lib -lssl -lcrypto
    + sudo ./server
    
 
