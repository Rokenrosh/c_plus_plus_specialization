#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  //UpdateDocumentBase(document_input);
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(current_document);
  }

  swap(index.GetAccess().ref_to_value, new_index);
}

void UpdateDocumentBaseSingleThread(istream& document_input, Synchronized<InvertedIndex>& index){
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(current_document);
  }

  swap(index.GetAccess().ref_to_value, new_index);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  futuries.push_back(async(UpdateDocumentBaseSingleThread, ref(document_input), ref(index)));
}

void AddQueriesStreamSingleThread(
  istream& query_input, ostream& search_results_output, Synchronized<InvertedIndex>& index
) {
  int size = index.GetAccess().ref_to_value.GetCount();
  vector<int> docid_count(size);
  vector<int> indexes(size);

  for (string current_query; getline(query_input, current_query); ) {
    for(int i = 0; i < size; i++){
      docid_count[i] = 0;
      indexes[i] = i;
    }
    const vector<string> words = SplitIntoWords(current_query);

    for (const auto& word : words) {
      for (const pair<int, int>& docid : index.GetAccess().ref_to_value.Lookup(word)) {
        docid_count[docid.first] += docid.second;
      }
    }
    partial_sort(
      indexes.begin(),
      indexes.begin() + min(5, size),
      indexes.end(),
      [&docid_count](int lhs, int rhs) {
        return make_pair(docid_count[lhs], -lhs) > make_pair(docid_count[rhs], -rhs);
      }
    );
    search_results_output << current_query << ':';
    for (auto docid : Head(indexes, 5)) {
      if(docid_count[docid] == 0){
        continue;
      }
      search_results_output << " {"
        << "docid: " << docid << ", "
        << "hitcount: " << docid_count[docid] << '}'; 
    }
    search_results_output << "\n";
  }
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output){
  futuries.push_back(async(AddQueriesStreamSingleThread, ref(query_input), ref(search_results_output), ref(index)));
}

void InvertedIndex::Add(const string& document) {
  docs.push_back(document);

  const int docid = docs.size() - 1;
  for (const auto& word : SplitIntoWords(docs.back())) {
    auto& current_word = index[word];
    if(current_word.empty() || current_word.back().first != docid){
      current_word.emplace_back(docid, 1);
    }
    else{
      current_word.back().second++;
    }
  }
}

const vector<pair<int, int>>& InvertedIndex::Lookup(const string& word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return empty_vector;
  }
}
