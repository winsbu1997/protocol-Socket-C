- 6 Giao thức TCP, UDP, ICMP, DNS, HTTP, HTTPS sử dụng socket
- client chạy trên window
- server chạy trên Ubuntu

Giao thức tự động gửi tin nhắn từ client -> server và ngược lại. kết thúc khi tin nhắn ("*")
Cách chạy:
  - Trên window tải openssl-1.1.0f-vs2017 
  - Chạy quyền Admin để gửi ICMP
  - Trên Ubuntu:
    + Cài đặt thư viện SSL : sudo apt-get install libssl-dev
    + gcc -Wall -o server all_server.c -L/usr/lib -lssl -lcrypto
    + sudo ./server
    
 
