/*
Modified from https://en.wikipedia.org/wiki/Circular_buffer
Mirroring version
On 18 April 2014, the simplified version on the Wikipedia page for power of 2 sized buffers 
doesn't work - cbIsEmpty() returns true whether the buffer is full or empty.
*/

/** Circular buffer object.  Has a fixed number of cells, set to 256.
@tparam ITEM_TYPE the kind of data to store, eg. int, int8_t etc.
*/
template <class ITEM_TYPE>
class CircularBuffer
{

public:
	/** Constructor
	*/
	CircularBuffer(): start(0),end(0),s_msb(0),e_msb(0)
	{
	}

	inline
	bool isFull() const {
		return end == start && e_msb != s_msb;
	}

	inline
	bool isEmpty() const {
		return end == start && e_msb == s_msb;
	}

	inline
	void write(ITEM_TYPE in) {
		items[end] = in;
		//if (isFull()) cbIncrStart(); /* full, overwrite moves start pointer */
		cbIncrEnd();
	}

	inline
	ITEM_TYPE read() {
		ITEM_TYPE out = items[start];
		cbIncrStart();
		return out;
	}

	inline uint8_t available() const {
		if (isFull()) return 255;
		return end-start;
	}

	inline
	unsigned long count() const {
		return (num_buffers_read << 8) + start;
	}

private:
	ITEM_TYPE items[256];
	uint8_t         start;  /* index of oldest itement              */
	uint8_t         end;    /* index at which to write new itement  */
	uint8_t         s_msb;
	uint8_t         e_msb;
	unsigned long num_buffers_read;


	inline
	void cbIncrStart() {
		start++;
		if (start == 0) {
			s_msb ^= 1;
			num_buffers_read++;
		}
	}

	inline
	void cbIncrEnd() {
		end++;
		if (end == 0) e_msb ^= 1;
	}

};

/**
* When CircularBuffer is not enough... This buffer template uses 16 bit indices, and 
can therefore be set to hold much more data. For size 256 use CircularBuffer, instead.
*
* @tparam ITEM_TYPE the kind of data to store, eg. int, int8_t etc.
* @tparam SIZE size of the buffer, up to 32768 items (if you have that much RAM). Note that for
* simplicity, this buffer can only be filled up to SIZE-1 items. Do not write to it, if isFull()
* returns true.
*/
template <class ITEM_TYPE, uint16_t SIZE>
class CircularBuffer16
{

public:
	/** Constructor
	*/
	CircularBuffer16(): start(0),end(0)
	{
	}

	inline
	bool isFull() const {
		return (end == start-1) || (start == 0 && end == SIZE-1);
	}

	inline
	bool isEmpty() const {
		return end == start;
	}

	inline
	void write(ITEM_TYPE in) {
		items[end] = in;
                if (++end >= SIZE) {
                    end = 0;
                }
	}

	inline
	ITEM_TYPE read() {
		ITEM_TYPE out = items[start];
		if (++start >= SIZE) {
                    start = 0;
                    ++readcount;
                }
		return out;
	}

	inline uint8_t available() const {
#warning WRONG
		return end-start;
	}

	inline
	unsigned long count() const {
		return (readcount*SIZE) + start;
	}
private:
	ITEM_TYPE items[SIZE];
	uint16_t         start;  /* index of oldest itement              */
	uint16_t         end;    /* index at which to write new itement  */
	unsigned long readcount;
};
