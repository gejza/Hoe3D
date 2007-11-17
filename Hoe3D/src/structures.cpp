#include "structures.h"

//-------------------------------------------------------
//	jednoduchy uzel
//-------------------------------------------------------
template <typename T> HoeSimpleNode<T>::HoeSimpleNode(){
	next=NULL;
}
/*
template <typename T> HoeSimpleNode<T>::HoeSimpleNode(T Value){
	setValue(Value);
	setNext(NULL);
}
*/
/*
template <typename T> HoeSimpleNode<T>::~HoeSimpleNode(){
		
}
*/
template <typename T> T* HoeSimpleNode<T>::getValue(){
	return &value;
}

template <typename T> HoeSimpleNode<T>* HoeSimpleNode<T>::getNext(){
	return next;
}

template <typename T> void HoeSimpleNode<T>::setValue(T &Value){
	value=Value;
}

template <typename T> void HoeSimpleNode<T>::setNext(HoeSimpleNode<T>* Next){	
	next=Next;
}

//-------------------------------------------------------
//	quad uzel
//-------------------------------------------------------
template <typename T> HoeQuadNode<T>::HoeQuadNode(){
	left=NULL;
	right=NULL;
	top=NULL;
	bottom=NULL;
}
template <typename T> HoeQuadNode<T>::HoeQuadNode(T &Value){
	setValue(Value);
	left=NULL;
	right=NULL;
	top=NULL;
	bottom=NULL;
}

template <typename T> HoeQuadNode<T>::~HoeQuadNode(){
}

template <typename T> T* HoeQuadNode<T>::getValue(){
	return &value;
}
template <typename T> HoeQuadNode<T>* HoeQuadNode<T>::getNext(int place){

	switch(place){
		case LEFT: return left;
		case RIGHT: return right;
		case TOP: return top;
		case BOTTOM: return bottom;
	}
	
	return NULL;
}

template <typename T> void HoeQuadNode<T>::setValue(T &Value){
	value=Value;	
}
template <typename T> void HoeQuadNode<T>::setNext(HoeQuadNode<T>* Node, int place){

	switch(place){
		case LEFT:
			left=Node;
			break;
		case RIGHT:
			right=Node;
			break;
		case TOP:
			top=Node;
			break;
		case BOTTOM:
			bottom=Node;
			break;
	}
	
}


//-------------------------------------------------------
//	LinkedList
//	spojovy seznam
//-------------------------------------------------------
template <typename T> HoeLinkedList<T>::HoeLinkedList(){	
	firstNode=NULL;
	actualNode=NULL;	
}

template <typename T> HoeLinkedList<T>::~HoeLinkedList(){

	static int d=0;
	HoeSimpleNode<T>* tmp;
	
	d++;
	printf("\n\tDESTRUKTOR %d\n",d);
	int n=1;
	while(firstNode!=NULL){

		printf("%d ",n++);
		tmp=firstNode->getNext();
		delete firstNode;
		firstNode=tmp;
		
	}
	printf("\n");
}

	//-------------------------------------------------------
	//	pridavani
	//-------------------------------------------------------
template <typename T> void HoeLinkedList<T>::add(T &value){

	HoeSimpleNode<T>* newNode=new HoeSimpleNode<T>();
	newNode->setValue(value);	

	if(firstNode==NULL){
		firstNode=newNode;
		actualNode=firstNode;
		lastNode=firstNode;
		return;
	}
	
	lastNode->setNext(newNode);
	lastNode=lastNode->getNext();
	
}

template <typename T> bool HoeLinkedList<T>::hasNext(){
			
	if(actualNode==NULL){
		actualNode=firstNode;
		return false;
	}
	return true;
}

template <typename T> T* HoeLinkedList<T>::getNext(){
	
	T* Value=actualNode->getValue();
	actualNode=actualNode->getNext();
	return Value;

}

//-------------------------------------------------------
//	quad list
//-------------------------------------------------------
template<typename T> HoeQuadList<T>::HoeQuadList<T>(){

	firstNode=NULL;
	actualNode=NULL;	
}

template<typename T> HoeQuadList<T>::~HoeQuadList<T>(){

}

template<typename T> void HoeQuadList<T>::add(T &value ,int place){
	
	HoeQuadNode<T>* newNode=new HoeQuadNode<T>();
	newNode->setValue(value);

	if(firstNode==NULL){
		firstNode=newNode;
		actualNode=firstNode;
		lastNode=firstNode;
		return;
	}
	
	lastNode->setNext(newNode, place);
	lastNode->getNext(place)->setNext(lastNode, ANTIDIR(place));
	lastNode=lastNode->getNext(place);
}