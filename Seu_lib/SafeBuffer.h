#pragma once

#include <vcruntime_string.h>
#include <string.h>

#define SAFE_DELETE(p) if(p){ delete (p); (p) = NULL; }

class SafeBuffer
{
public:
	SafeBuffer() : m_ref(new int(1)) {
		m_size = 0;
		m_data = NULL;
	}
	SafeBuffer(int initialSize) : m_ref(new int(1)) {
		m_size = initialSize < 16 ? 16 : initialSize;
		m_data = new char[m_size]();
	}
	static SafeBuffer From(const char* buf, int size) {
		SafeBuffer sBuf(size);
		memcpy(sBuf.m_data, buf, size);
		return sBuf;
	}
	bool CopyFrom(const char* buf, int size) {
		if (m_size >= size)
			memcpy(m_data, buf, size);
		return m_size >= size;
	}
	virtual ~SafeBuffer() {
		DelRef();
	}
	SafeBuffer(const SafeBuffer &o){
		m_size = o.m_size;
		m_data = o.m_data;
		m_ref = o.m_ref;
		AddRef();
	}
	// ¿½±´¸³ÖµÔËËã·û
	SafeBuffer& operator=(const SafeBuffer& o) {
		if (this != &o) {
			DelRef();
			m_size = o.m_size;
			m_data = o.m_data;
			m_ref = o.m_ref;
			AddRef();
		}
		return *this;
	}
	SafeBuffer SafeCheck(int size) {
		if (size > m_size) {
			SafeBuffer buf(size);
			buf.CopyFrom(m_data, m_size);
			*this = buf;
		}
		return *this;
	}
	operator const char*() const {
		return m_data;
	}
	operator char* &() {
		return m_data;
	}
	char* c_str() {
		return m_data;
	}
private:
	int m_size;
	int *m_ref;
	char *m_data;

	void AddRef() {
		m_ref ? ++(*m_ref) : 0;
	}
	void DelRef() {
		int ref = m_ref ? --(*m_ref) : 0;
		if (0 == ref) {
			m_size = 0;
			SAFE_DELETE(m_data);
			SAFE_DELETE(m_ref);
		}
	}
};
