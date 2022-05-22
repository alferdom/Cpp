#pragma once

class ElementBuffer
{
private:
	unsigned int RendererID;
	unsigned int Count;
public:
	ElementBuffer();
	ElementBuffer(const unsigned int* data, unsigned int count);
	~ElementBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int getCount() const {
		return Count;
	}
};
