#include <map>
#include <iostream>
#include <utility>
#include <type_traits>
using namespace std;

template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	template<typename V_forward>
	interval_map(V_forward&& val)
	: m_valBegin(std::forward<V_forward>(val))
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	template<typename V_forward>
	void assign( K const& keyBegin, K const& keyEnd, V_forward&& val )
		requires (std::is_same<std::remove_cvref_t<V_forward>, V>::value)
	{
// INSERT YOUR SOLUTION HERE
		if (!(keyBegin < keyEnd)) 
		{
			return;
		}

		auto itLow = m_map.lower_bound(keyBegin);
		auto itHigh = m_map.upper_bound(keyEnd);

		V prevValue = (itLow == m_map.begin()) ? m_valBegin : std::prev(itLow) -> second;
		V nextValue = (itHigh == m_map.end()) ? m_valBegin : itHigh -> second;

		if (prevValue == val && nextValue == val) 
		{
			return;
		}

		m_map.erase(itLow, itHigh);

		if (prevValue != val) 
		{
			m_map[keyBegin] = val;
		}

		if (nextValue != val)
		{
			m_map[keyEnd] = nextValue;
		}
	}

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.

void test() {
	interval_map<int, char> m('A');
	m.assign(1, 3, 'B');
	m.assign(4, 6, 'C');
	m.assign(2, 5, 'D');

	for (int i = -1; i <= 7; ++i) 
	{
		std::cout << i << " -> " << m[i] << "\n";
	}
}

int main() {
	test();
	return 0;
}