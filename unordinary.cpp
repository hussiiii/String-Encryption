#include <iostream>
#include <algorithm>
#include <cctype> // for ::tolower
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void Decrypt(string& str);
void RemoveXs(string& str);
void Unscramble(string& str);
void ExtractAndStoreLastLetters(string& str1, string& str2);
void BringToFrontAndAttachLettersToEnd(string& str1, string& str2);


int main()
{
    string entry = "iXaaEEaS TMXBO eeBSeESSbEOE faITWD UabaTURFHXfROOFfdIeaNPE G. iXeeLWeAYA EfeSaaIfTATHSXbUTaTINGPE daOLAXOT abIMTFXNAbfE fdFFOdRED NbTdIT OMeeETeHfINSOXIG fHdERTFXIE OffNSXAT UfTfbebCOMONXHdfATE dbILWT AbbSLL NbTdIeUTXi IDabL E. HERT dReAE WdTeEXdMbfPeORTAaNIO bAdeKETTXdEeRHS E. HTXIaRaSdFE IT eaHATS HbaEWTXi ANXdfNVfESTfINGIM OadLAXOT IMafTFXNeAeE bFfFOaRED NbTbIT dOeMfEbTHbINSOXIG dbHOULeST BD OdbMEdTHIbNSEaXdaHATG ITXffRfULTS ebOdRTWYXaHAbTH bIdMTT aNAaE FFedObRED T. HaeESTXbHdINGdTE RebASXUTbURfaFE abROOPfEXBF deATbURNY E. eIaKLXHbdTE bUadTCbbOMOEXOE HITF adaOdURdNAJSa faOFLXfbNSTAdaNCIR E. HIdfTXeOdURbNdAJS AbMfLXBY feOSLEXUBT fHTTXbUdTCOMOE bILWEXAdSLL ORfEVEfFT R. HTXfdTHEOE eAKbTRXIE HedATSXdOMST HIaNGfdTEXdUSJS RAfTXaObNE EANfMTXTT eASLO T. ONXfVEbRdYTHeINET aEEDffNG ebOMSS IeNdKEXOD EEPSaAKfKFXTaE ETbeAfIROXAdLdUeVN E. bdNTIabU EKeffSNL dbIMTT E.  ";
    Decrypt(entry);
    cout << entry << endl;

    return 0;
}

void Decrypt(string& entry)
{
    string storedLetters = "";

    // Step 1: Remove Capital 'X' between words and replace with space
    RemoveXs(entry);


    // Step 2: Unscramble (remove all lowercase letters except i) 
    Unscramble(entry);

    //Step 3: Extract and store last letters of all words excpet the first one of each "section" 
    ExtractAndStoreLastLetters(entry, storedLetters);

    //Step 4: Iterate through entry, and bring the last letter of each word to the front of the word and then add the first character of storedLetters
    //to the end of the word, then pop_front storedLetters. Each time you attach a letter from the front of storedLetters, check to see if the next char 
    //in storedLetters is a puncutation mark. if it is, add that too to that word and then pop_front that too
    BringToFrontAndAttachLettersToEnd(entry, storedLetters);

}

void RemoveXs(string& str)
{
    replace(str.begin(), str.end(), 'X', ' ');
}

void Unscramble(string& word)
{
    // Remove all lowercase letters from the word, except 'i'
    word.erase(remove_if(word.begin(), word.end(), [](char c) { return islower(static_cast<unsigned char>(c)) && c != 'i'; }), word.end());
}

void ExtractAndStoreLastLetters(string& entry, string& storedLetters)
{
    bool firstWord = true;
    string word = "";
    storedLetters = "";
    for (int i = 0; i <= entry.size(); i++)
    {
        if (entry[i] == 'i')
        {
            word = word + entry[i] + ' ';
            i++;
        }
        else if (entry[i] != ' ' && entry[i] != '?' && entry[i] != '.' && entry[i] != '!' && entry[i] != ',' && entry[i] != ':')
        {
            word = word + entry[i];
        }
        else if (entry[i] == ' ')
        {
            if (firstWord == true)
            {
                firstWord = false;
                word = word + entry[i];
            }
            else
            {
                storedLetters += word.back();
                word.pop_back();
                word = word + ' ';
            }
        }
        else if (entry[i] == '?' || entry[i] == '.' || entry[i] == '!' || entry[i] == ',' || entry[i] == ':')
        {
            storedLetters += word.back();
            word.pop_back();
            storedLetters += entry[i];
            firstWord = true;
            i++;
        }

    }
    entry = word;
}

void BringToFrontAndAttachLettersToEnd(string& entry, string& storedLetters)
{
    stringstream ss(entry);
    string word, result;
    while (ss >> word)
    {
        if (word == "i")
        {
            // If the word is a lowercase "i", skip the process
            // of attaching the next character in storedLetters to it
            result += word + ' ';
            continue;
        }

        if (word.length() > 1)
        {
            char lastLetterToFront = word.back();
            word.pop_back();
            word = lastLetterToFront + word;
        }

        if (!storedLetters.empty())
        {
            word += storedLetters[0];
            storedLetters.erase(storedLetters.begin());

            if (!storedLetters.empty() && ispunct(storedLetters[0]))
            {
                word += storedLetters[0];
                storedLetters.erase(storedLetters.begin());
            }
        }

        result += word + ' ';
    }
    entry = result;
}




