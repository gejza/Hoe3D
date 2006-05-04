#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#define LEFT 1
#define RIGHT 2
#define TOP 3
#define BOTTOM 4
#define ANTIDIR(DIR) DIR + DIR%2 - (DIR+1)%2

//-------------------------------------------------------
//	typy uzlu
//-------------------------------------------------------
template<typename T> class HoeSimpleNode{

private:
	T value;
	HoeSimpleNode<T>* next;

public:
	HoeSimpleNode<T>();
	//HoeSimpleNode(T Value);
	//~HoeSimpleNode<T>();

	T* getValue();
	HoeSimpleNode<T>* getNext();

	void setValue(T &Value);
	void setNext(HoeSimpleNode<T>* Next);
};

template<typename T> class HoeDoubleNode{

private:
	T value;
	HoeDoubleNode<T>* left, right;

public:
	HoeDoubleNode();
	~HoeDoubleNode();

	T* getValue();	
	HoeDoubleNode<T>* getNext(int place);

	void setValue(T &Value);
	void setNext(HoeDoubleNode<T>* Left, int place);	
};

template<typename T> class HoeQuadNode{

private:
	T value;
	HoeQuadNode<T>* left,* right,* top,* bottom;

public:
	
	HoeQuadNode<T>();
	HoeQuadNode<T>(T &Value);
	~HoeQuadNode<T>();

	T* getValue();
	HoeQuadNode<T>* getNext(int place);

	void setValue(T &Value);
	void setNext(HoeQuadNode<T>* Left, int place);	
};

//-------------------------------------------------------
//	LinkedList
//	spojovy seznam
//-------------------------------------------------------
template<typename T> class HoeLinkedList{

private:
	
	HoeSimpleNode<T>* firstNode;
	HoeSimpleNode<T>* lastNode;
	HoeSimpleNode<T>* actualNode;

public:
	HoeLinkedList<T>();	
	~HoeLinkedList<T>();
	void add(T &value);	
	bool hasNext();
	T* getNext();

};

//-------------------------------------------------------
//	QuadList
//-------------------------------------------------------
template<typename T> class HoeQuadList{

private:
	
	HoeQuadNode<T>* firstNode;
	HoeQuadNode<T>* lastNode;
	HoeQuadNode<T>* actualNode;

public:

	HoeQuadList<T>();
	~HoeQuadList<T>();		
	void add(T &value, int place);
	void next();
	T* getNext(int place);
};


//-------------------------------------------------------
//	QuadTree
//-------------------------------------------------------
template<typename T> class HoeQuadTree{

private:

	HoeQuadNode<T>* firstNode;


};



#endif
