

using namespace std;

class BitStream {
    private:
        fstream file;
        size_t readPos;
        size_t writePos;

    public:
        BitStream(char* path) {
            file = fstream{path};
            file.seekg(file.beg);   // read: first byte
            readPos = 0;
            file.seekp(file.end);   // write: last byte
            writePos = 0;
        }

        bool readBit();
        void writeBit(bool bit);
        vector<bool> readNBits(size_t N);
        void writeNBits(vector<bool> bits);
        string readString();
        void writeString(char* s);

};