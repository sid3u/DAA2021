#include "pch.h"

#include "NodeSearch.h"

void NodeSearch::setIntensity(int intensity) {

	if (intensity < 0) {

		throw logic_error("intensite negative");
	}

	_intensity = intensity;
}

void NodeSearch::setCall(string str){
	if(str.find("		call	ds:") != string::npos){
		_isCall = true;
	}	
}

void NodeSearch::decreaseIntensity() {

	try {
		setIntensity(getIntensity() - 1);
	}
	catch (logic_error) {
	}
}

NodeSearch::NodeSearch(const string target, Node *node) : _target(target), _node(node), _intensity(0) {}

NodeSearch::NodeSearch(const string target) : NodeSearch(target, NULL) {}

const int NodeSearch::getIntensity() const {

	return _intensity;
}

const bool NodeSearch::getCall() const {
	return _isCall;
}

const string NodeSearch::getTarget() const {

	return _target;
}
const Node *NodeSearch::getNode() const {

	return _node;
}

vector<Node*> NodeSearch::getDescendants() {

	vector<Node*> descendant;

	if (!isLeaf()) {

		descendant.push_back(_node);
	}

	return descendant;
}

void NodeSearch::search(string str) {

	if (!isLeaf() && getIntensity() > 0) {

		_node->search(str);
	}
	else if (str.find(getTarget()) != string::npos) {


		setIntensity(ACTIVATION_INTENSITY);
		setCall(str);
	}

	decreaseIntensity();
}

void NodeSearch::searchString(string str) {

	if (!isLeaf() && getIntensity() > 0) {

		_node->search(str);
	}
	else if (str.find(getTarget()) != string::npos) {

		if(verificar(str, getTarget())){
			setIntensity(ACTIVATION_INTENSITY);
		}
		setCall(str);
	}

	decreaseIntensity();
}

const bool NodeSearch::isActivate() const {

	if (isLeaf()) {

		return getIntensity() > 0;
	}

	return getNode()->isActivate();
}

const bool NodeSearch::isCall() const{
	return getCall();
}


const bool NodeSearch::isLeaf() const {

	return getNode() == NULL;
}


NodeSearch::operator string() const {

	ostringstream os;

	os << "target=" << getTarget() << ", intensity=" << getIntensity() << ", node=" << (*_node);

	return os.str();
}


const bool NodeSearch::verificar(string phrase, string mot) const{ 
	
	/*int pos = phrase.find(mot);   
    std::regex const pattern{ "[a-zA-Z0-9]+" };
    if (pos - 1 > -1) {      
       if(std::regex_match(&phrase[pos - 1] , pattern)) { return false; }   
    }
    else if (pos + mot.size() > phrase.size() - 1) {
        if (std::regex_match(&phrase[pos + mot.size()], pattern)) { return false; }
    }

	return true;   */

	int pos = phrase.find(mot);
    string premiere = "";
    string derniere = "";
 
   std::regex const pattern{ "[[:alnum:]]" };

    if (pos - 1 > -1) {
         premiere += phrase[pos - 1];
        if (std::regex_match(premiere, pattern)) {  return false; }
    }


    if (pos + mot.size() <= phrase.size()) {

        derniere += phrase[pos + mot.size()];
        if (std::regex_match(derniere, pattern)) { return false; }
    }
    return true;
}
