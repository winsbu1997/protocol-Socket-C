6 Giao thức TCP, UDP, ICMP, DNS, HTTP, HTTPS sử dụng socket. Client chạy trên window sử dụng Visual Studio 2017, server chạy trên Ubuntu sử dụng terminal

Giao thức tự động gửi tin nhắn từ client -> server và ngược lại. kết thúc khi tin nhắn (*).                                     Đổi các đường dẫn tới file Process_Hidden trong code CLient
Link ProcessHider của tác giả kernelm0de - https://github.com/kernelm0de/ProcessHider
# Cách chạy: 
  - Chạy quyền Admin để gửi ICMP
  - Trên window 
    + tải openssl-1.1.0f-vs2017 trên https://www.npcglib.org/~stathis/blog/precompiled-openssl/
    + Config trên visual studio
      Chọn Project -> Properties 
        - C/C++ -> general -> Additional Include Directories -> ..\openssl-1.1.0f-vs2017\include
        - Linker -> general -> Additional Include Directories -> ..\openssl-1.1.0f-vs2017\lib
        - Input -> thêm 2 mục lib vào Additional Dependencies (libsslMT.lib;libcryptoMT.lib;).
    
  - Trên Ubuntu:
    + Cài đặt thư viện SSL : sudo apt-get install libssl-dev
    + gcc all_server.c -o server -L/usr/lib -lssl -lcrypto
    + sudo ./server
    
 
