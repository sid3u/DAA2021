#include "pch.h"

#ifndef DEF_NODE_SEARCH
#define DEF_NODE_SEARCH

#include "Node.h"

using namespace std;

class NodeSearch : public Node {

private:

	Node *_node;
	string _target;
	int _intensity;
	bool _isCall = false;

	void setIntensity(int intensity);
	void setCall(string str);
	void decreaseIntensity();

public:

	const int ACTIVATION_INTENSITY = 17;
 
	NodeSearch(const string target, Node *node);
	NodeSearch(const string target);

	const Node *getNode() const;
	const string getTarget() const;
	const int getIntensity() const;
	const bool getCall() const;

	vector<Node*> getDescendants();
	void search(string str);
	const bool isActivate() const;
	const bool isCall() const;

	const bool isLeaf() const;

	operator string() const;
};

inline ostream &operator<<(ostream &os, const NodeSearch &nodeSearch) {

	os << (string) nodeSearch;

	return os;
}

#endif