
#include <iostream>
#include<windows.h>

using namespace std;

struct SongData {
    char tag[3];    
    char songName[30]; 
    char artist[30]; 
    char album[30]; 
};


void write(HANDLE  fHandle, const SongData& songData) {
    const int ID3_size = 128;

    DWORD bytesWritten;

    SetFilePointer(fHandle, -ID3_size, NULL, FILE_END);
    WriteFile(fHandle, &songData, sizeof(songData), &bytesWritten, NULL);

    CloseHandle(fHandle);

}



void read( const string& path)
{
    wstring widePath(path.begin(), path.end());
    HANDLE failas = CreateFile(widePath.c_str(), GENERIC_READ |GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 

    if (failas == INVALID_HANDLE_VALUE) {
       cout << "Neatidaro failo (klaida ties CreateFile)" << endl;
        return;
    }

    SongData songData;
    DWORD dydis =  GetFileSize(failas, NULL); 
    const int ID3_size = 128;
    if (dydis == INVALID_FILE_SIZE) {
        cout << "nenuskaito failo dydzio (klaida ties GetfileSize)" << endl;
        CloseHandle(failas);
        return;
    }


    LPVOID buffer = VirtualAlloc(NULL, dydis, MEM_COMMIT, PAGE_READWRITE);
        
    if (buffer == NULL) {
        cout << "Nepavyksta isskirti atminties, (kalida ties VirtualAloc)" << endl;
        CloseHandle(failas);
        return;
    }
    
    SetFilePointer(failas, -ID3_size, NULL, FILE_END);

    DWORD fileBytes;
    if (!ReadFile(failas, buffer, dydis, &fileBytes, NULL)) {
        cout << "nepavyksta nuskaityti turinio, (klaida ties Readfile )." << endl;
        VirtualFree(buffer, 0, MEM_RELEASE);
        CloseHandle(failas);
        return;
   }

    unique_ptr<char[]> id3Buffer(new char[ID3_size]);

    for (int i = 0; i < ID3_size; ++i) {
        id3Buffer[i] = static_cast<char*>(buffer)[i];
    }


    for (int i = 0; i < ID3_size; ++i) {
        if (i < 3) {
            songData.tag[i] = id3Buffer[i];
        }
        else if (i >= 3 && i < 33) {
            songData.songName[i - 3] = id3Buffer[i];
        }
        else if (i >= 33 && i < 63) {
            songData.artist[i - 33] = id3Buffer[i];
        }
        else if (i >= 63 && i < 93) {
            songData.album[i - 63] = id3Buffer[i];
        }
    }
   
   
    bool updated = false;
    while (!updated) {

        cout << "\n Pavadinimas: " << string(songData.songName, 30) << endl;
        cout << "Atlikėjas: " << string(songData.artist, 30) << endl;
        cout << "Albumas: " << string(songData.album, 30)<< '\n' << endl;

        cout << " 1 | Pavadinimas  " << endl;
        cout << " 2 | Atlikėjas  " << endl;
        cout << " 3 | Albumas  " << endl;
        cout << " 4 | issaugoti  " << endl;
        cout << " 5 | atsaukti " << endl;

        int input;
        cin >> input;

        switch (input) {
        case 1:
            cout << "Naujas pavadinimas: ";
            cin.ignore();
            memset(songData.songName, '\0', sizeof(songData.songName));
            cin.getline(songData.songName, 30, '\n');
            break;
        case 2:
            cout << "Naujas atlikejas: ";
            cin.ignore();
            memset(songData.artist, '\0', sizeof(songData.artist));
            cin.getline(songData.artist, 30, '\n');
            break;
        case 3:
            cout << "Naujas albumas: ";
            cin.ignore();
            memset(songData.album, '\0', sizeof(songData.album));
            cin.getline(songData.album, 30, '\n');
            break;
        case 4:
            write(failas, songData);
            updated = true;
            cout << "issaugota \n";
            break;
        case 5:
            cout << "duomenu keitimas atsauktas \n" << endl;
            updated = true;
            break;
        default:
            cout << "blogas pasirinkimas \n" << endl;
            break;
        }
     
  
    }

    std::cout << "File size: " << dydis << " bytes" << std::endl;
        
    VirtualFree(buffer, 0, MEM_RELEASE);
    CloseHandle(failas);
    return;
}


int main()
{
    
    string filePath = "C:\\Users\\Pijus\\source\\repos\\sisteminisLD2\\bensound-far.mp3";
    read(filePath);

}
