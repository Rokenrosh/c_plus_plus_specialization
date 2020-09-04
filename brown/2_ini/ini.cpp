#include "ini.h"

namespace Ini{

Section& Document::AddSection(string name){
    return sections[move(name)];
}

const Section& Document::GetSection(const string& name) const{
    return sections.at(name);
}

size_t Document::SectionCount() const{
    return sections.size();
}

Document Load(istream& input){
    Document doc;
    string line;
    Section* section;
    while(getline(input, line)){
        if(line.empty()){
            continue;
        }
        if(line[0] == '[' && line[line.length() - 1] == ']'){
            section = &doc.AddSection(move(line.substr(1, line.length() - 2)));
        }
        else{
            size_t pos = line.find_first_of("=");
            section->insert({move(line.substr(0, pos)), move(line.substr(pos+1))});
        }
    }
    return doc;
}
}