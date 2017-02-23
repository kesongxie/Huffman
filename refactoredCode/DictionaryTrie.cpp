/**
 *  CSE 100 PA2 C++ Autocomplete
 *  Authors: Kesong Xie, Jor-el Briones, Christine Alvarado
 */
#include "util.h"
#include "DictionaryTrie.h"

/** Node::Node constrctor for the Node class
 * @param char data: the data that the given node holds
 * @param bool isWordEnd: a flag that indicates whether the given node is a ending word node, true
 * if it is, false otheriwse
 * @param unsigned int freq: the frequency that a given word appears, which is only
 * applicable to the word node
 */
Node::Node(char data, bool isWordEnd, unsigned int freq){
    this->data = data;
    this->isWordEnd = isWordEnd;
    this->freq = freq;
    this->left = NULL;
    this->right = NULL;
    this->middle = NULL;
}

/** DictionaryTrie::DictionaryTrie constrctor for the DictionaryTrie class
 *  The constructor is responsible for initializing root to NULL
 */
DictionaryTrie::DictionaryTrie(){
    root = NULL;
}

/* DictionaryTrie::insert: Insert a word with its frequency into the dictionary.
 * @param std::string word: the string data that will store in the newly creatd node
 * @param unsigned int freq: the corresponding frequency for the given word, if the word is existed
 * in the trie already, and the the trie will update the correspoding node with the bigger frequency
 * between the old frequency the the receiving new frequency
 * @return bool: returns true if the word was inserted, and false if it
 * was not (i.e. it was already in the dictionary or it was
 * invalid (empty string) */
bool DictionaryTrie::insert(std::string word, unsigned int freq)
{
    //make sure the word is not empty
    if(word.empty()){
        return false;
    }
    
    //The current tree is empty
    if(!root){
        root = createNode(word, 0, freq);
        Node* p = root;
        for(unsigned int i = 1; i < word.length(); i++){
            p->middle = createNode(word, i, freq);
            p = p->middle;
        }
        return true;
    }
    
    /*The current tree is not empty*/
    unsigned int i = 0;
    Node* p = root;
    while(i < word.length()){
        if(p != NULL){
            if(word[i] == p->data){
                //check whether this is the ending char
                if(i == word.length() - 1){
                    p->freq = std::max(p->freq, freq);
                    if(p->isWordEnd){
                        //already existed
                        //update the frequence
                        return false;
                    }else{
                        p->isWordEnd = true; //finish the insertion for existing node
                        return true;
                    }
                }else{
                    //this is not the ending char
                    i++;
                    if(p->middle == NULL){
                        p->middle = createNode(word, i, freq);
                        p = p->middle;
                        i++;
                        break;
                        //finish the middle chain outside this while loop
                    }else{
                        p = p->middle;
                    }
                }
            }else if(word[i] > p->data){
                if(p->right == NULL){
                    p->right = createNode(word, i, freq);
                    p = p->right;
                    i++;
                    break;
                    //finish the middle chain outside this while loop
                }else{
                    p = p->right;
                }
            }else{
                if(p->left == NULL){
                    p->left = createNode(word, i, freq);
                    p = p->left;
                    i++;
                    break;
                    //finish the middle chain outside this while loop
                }else{
                    p = p->left;
                }
            }
        }
    }
    
    /*complete the middle chain for the left over chars, if the loop breaks early*/
    while(i < word.length()){
        p->middle = createNode(word, i, freq);
        p = p->middle;
        i++;
    }
    return true;
}


/* DictionaryTrie::createNode: a convenient helper function for creating a node
 * @param word: the word that the node created from
 * @param index: the index for the char in the word, that is used for the data for the node
 * @param freq: the frequence for the word, this value is used only when the char at the given index is the last char of the word
 * @return Node*: the node being created
 */
Node* DictionaryTrie::createNode(std::string word, unsigned int index, unsigned freq){
    //default false if this is not the last char
    bool isWordEnd = false;
    //default 0 if this is not the index for the last char
    unsigned frequence = 0;
    if(index == word.length() - 1){
        /*this is the last char*/
        isWordEnd = true;
        frequence = freq;
    }
    return new Node(word[index], isWordEnd, frequence);
}



/** DictionaryTrie::find: check the existence of a given word in the dictionary trie.
 *  @param std::string word: the word to search for
 *  @return bool: returns true if the word is in the dictionary and false otherwise: it does not care about the
 *  word's frequency
 */
bool DictionaryTrie::find(std::string word) const
{
    return getWord(word) != NULL;
}


/** DictionaryTrie::getWord: get the node reference to a given word
 *  @param std::string word: the word to search for
 *  @return Node*: returns the node reference for a given word if it's existed in the 
 *  trie, or return NULL if not found in the trie
 */
Node* DictionaryTrie::getWord(std::string word) const{
    if(word.empty() || root == NULL){
        return NULL;
    }
    Node* p = root;
    unsigned int i = 0;
    while(i < word.length()){
        if(p != NULL){
            if(p->data == word[i]){
                if(++i < word.length()){
                    p = p->middle;
                }
            }else if (word[i] > p->data){
                p = p->right;
            }else{
                p = p->left;
            }
        }else{
            return NULL;
        }
    }
    return p;
}

/* DictionaryTrie::predictCompletions: Return up to num_completions of the most frequent completions
 * of the prefix, such that the completions are words in the dictionary.
 * These completions should be listed from most frequent to least.
 * If there are fewer than num_completions legal completions, this
 * function returns a vector with as many completions as possible.
 * If no completions exist, then the function returns a vector of size 0.
 * The prefix itself might be included in the returned words if the prefix
 * is a word (and is among the num_completions most frequent completions
 * of the prefix)
 * @param std::string prefix: the word prefix to search for
 * @param unsigned int num_completions: the maximum number of prediction results that the 
 * function should return
 * @return std::vector<std::string>: return a vector that contains all the matched strings for the 
 * given prefix
 */
std::vector<std::string> DictionaryTrie::predictCompletions(std::string prefix, unsigned int num_completions)
{
    std::vector<std::string> words;

    //validate prefix
    //out put "Invalid Input. Please retry with correct input" if prefix is invalid
    for(unsigned int i = 0; i < prefix.length(); i++){
        if( (prefix[i] < 'a' || prefix[i] > 'z') && (prefix[i] < 'A' || prefix[i] > 'Z') && (prefix[i] < '0' || prefix[i] > '9') && prefix[i] != ' '){
            std::cout << "Invalid Input. Please retry with correct input" << std::endl;
            return words;
        }
    }
    
    //the prefix is valid
    std::vector<std::pair<std::string, int>> completionResult;
    Node* prev = NULL;
    Node* p = root;
    unsigned int i = 0;
    while(i < prefix.length()){
        if(p != NULL){
            if(prefix[i] == p->data){
                prev = p;
                p = p->middle;
                i++;
            }else if(prefix[i] > p->data){
                p = p->right;
            }else{
                p = p->left;
            }
        }else{
            prev = NULL;
            break;
        }
    }
    
    if(prev != NULL){
        if(prev->isWordEnd){
            //add the prefix itself to the completionResult
            completionResult.push_back(std::pair<std::string, int>(prefix, prev->freq));
        }

        inOrderTraversalAux(prev->middle, prefix, completionResult);
        //sort the words based on the frequency
        std::sort(completionResult.begin(), completionResult.end(), [](const std::pair<std::string,int> &left, const std::pair<std::string,int> &right) {
            return left.second > right.second;
        });
        
        if(completionResult.size() < num_completions){
            //return all the completion results
            for(auto word : completionResult){
                words.push_back(word.first);
            }
        }else{
            for(unsigned int i = 0; i < num_completions; i++){
                words.push_back(completionResult[i].first);
            }
        }
    }
    return words;
}

/*
 * inOrderTraversal: in order traversal
 * @return std::vector<std::pair<std::string, int>>: all the words with its frequency
 */
std::vector<std::pair<std::string, int>> DictionaryTrie::inOrderTraversal(){
    std::vector<std::pair<std::string, int>> words;
    inOrderTraversalAux(root, "", words);
    return words;
}

/*
 * inOrderTraversalAux: in order traversal, output results to words argument
 * @param Node* node: the starting node for post order traversal
 * @param std::string prefix: prefix for this given node
 * @param std::vector<std::pair<std::string, int>> & words: the container for saving output results
 * @return void
 */
void DictionaryTrie::inOrderTraversalAux(Node* node, std::string prefix, std::vector<std::pair<std::string, int>> & words){
    if(node != NULL){
        inOrderTraversalAux(node->left, prefix, words);
        if(node->isWordEnd){
            std::pair<std::string, int> pair(prefix+node->data, node->freq);
            words.push_back(pair);
        }
        //The node->data belongs to part of the middle subtree
        inOrderTraversalAux(node->middle, prefix + node->data, words);
        inOrderTraversalAux(node->right, prefix, words);
    }
}

/** DictionaryTrie::~DictionaryTrie: the destructor for the DictionaryTrie class, which is repsonsible
 *  for deallocated any dynamically memory associated with the class. This destructor would call the 
 *  recurssive postOrderDeleteAux for cleaning up the dynamically allocated memory
 */
DictionaryTrie::~DictionaryTrie(){
    postOrderDeleteAux(root);
}

/* postOrderDeleteAux: post order deallocate memory for nodes
 * @param Node* node: the starting node for post order traversal deletion
 * @return void
 */
void DictionaryTrie::postOrderDeleteAux(Node* node){
    if(node != NULL){
        postOrderDeleteAux(node->left);
        postOrderDeleteAux(node->middle);
        postOrderDeleteAux(node->right);
        delete node;
    }
}
