#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
using namespace std;

// Helper: Send a message to Firefox via Native Messaging protocol
void sendNativeMessage(const string& json) {
    uint32_t len = json.length();
    fwrite(&len, 4, 1, stdout);         // First 4 bytes: length
    fwrite(json.c_str(), 1, len, stdout); // Then the actual message
    fflush(stdout);
}

int main() {
    const int EDGE_THRESHOLD = 1;
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);

    while (true) {
        POINT p;
        if (GetCursorPos(&p)) {
            if (p.x <= EDGE_THRESHOLD) {
                sendNativeMessage(R"({"action":"switch-left"})");
                this_thread::sleep_for(chrono::milliseconds(500));
            } else if (p.x >= screenWidth - EDGE_THRESHOLD) {
                sendNativeMessage(R"({"action":"switch-right"})");
                this_thread::sleep_for(chrono::milliseconds(500));
            }
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    return 0;
}
