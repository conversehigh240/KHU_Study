#ifndef _TEXT_EDITOR_H_
#define _TEXT_DEITOR_H_
#include <string>
#include <iostream>
using namespace std;
typedef char ItemType;

struct LineType
{
	string data;
	LineType* prev;
	LineType* next;
};

class TextEditor
{
public:
	TextEditor();
	~TextEditor();

	int InsertItem(string _data);
	int GetLength();
	void ResetList();
	int GetNextItem(string& _data);
	
	void GoToTop(LineType *&linePtr);
	void GoToBottom(LineType *&linePtr);
	void MakeEmpty();
	void Print();

protected:
	int m_length;
	LineType* m_head;
	LineType* m_tail;
	LineType* m_curPos;
};

TextEditor::TextEditor()
{
	m_head = new LineType;
	m_tail = new LineType;
	m_curPos = m_head;

	m_head->prev = NULL;
	m_head->next = m_tail;
	m_tail->prev = m_head;
	m_tail->next = NULL;
	
	m_head->data = "Top";
	m_tail->data = "Bottom";

	m_length = 0;
}

TextEditor::~TextEditor()
{
	MakeEmpty();
}

void TextEditor::MakeEmpty()
{
	LineType* tempPtr;
	m_curPos = m_head->next;
	while(m_curPos != m_tail)
	{
		tempPtr = m_curPos;
		m_curPos->prev->next = m_curPos->next;
		m_curPos->next->prev = m_curPos->prev;
		m_curPos = m_curPos->next;
		delete tempPtr;
	}
	m_curPos = m_head;
	m_length = 0;
}

int TextEditor::InsertItem(string _data)
{
	LineType* newNode;
	newNode = new LineType;
	newNode->data = _data;

	newNode->prev = m_curPos;
	newNode->next = m_curPos->next;
	
	m_curPos->next = newNode;
	m_curPos = m_curPos->next;
	m_length++;
	return 1;
}

int TextEditor::GetLength()
{
	return m_length;
}

void TextEditor::ResetList()
{
	m_curPos = m_head;
}

int TextEditor::GetNextItem(string& _data)
{
	if( m_curPos->next == m_tail )
		return -1;

	m_curPos = m_curPos->next;
	_data = m_curPos->data;

	return 1;
}

void TextEditor::GoToTop(LineType *&linePtr)
{
	while (m_curPos->prev != NULL)
	{
		m_curPos = m_curPos->prev;
	}
	linePtr = m_curPos;
}

void TextEditor::GoToBottom(LineType *&linePtr)
{
	while (m_curPos->next != NULL)
	{
		m_curPos = m_curPos->next;
	}
	linePtr = m_curPos;
}

void TextEditor::Print()
{
	GoToTop(m_curPos);
	while (m_curPos != NULL)
	{
		std::cout << m_curPos->data << std::endl;
		m_curPos = m_curPos->next;
	}
}

#endif