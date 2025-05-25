#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

// Send a JSON message to Firefox
void sendNativeMessage(const string& json) {
    uint32_t len = json.length();
    fwrite(&len, 4, 1, stdout);
    fwrite(json.c_str(), 1, len, stdout);
    fflush(stdout);
}

// Dummy function to read stdin and keep Firefox connection alive
void inputListener() {
    while (true) {
        uint32_t len;
        if (!fread(&len, 4, 1, stdin)) break;

        string input(len, '\0');
        if (!fread(&input[0], 1, len, stdin)) break;

        cerr << "Received from Firefox (optional): " << input << endl;
    }
}

int main() {
    const int EDGE_THRESHOLD = 1;
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);

    // Launch thread to keep Firefox connection alive
    thread readerThread(inputListener);

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

    readerThread.join();
    return 0;
}
