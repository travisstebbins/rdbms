#include <string>

using namespace std;

class varchar
{
	string data;
	int limit;

	public:
		varchar(int _limit);
		string getString() { return data; }
		void setString(string s);
};

class Container {
	public:
		enum Type {
			VARCHAR,
			INTEGER
		};

		template<typename T>
		Container(Type type, T value);

		template<typename T>
		T getData();

	private:
		union Data {
			varchar _varchar;
			int _int;
		} data;

		Type _type;

};

template<typename T>
Container::Container(Type type, T value)
{
	_type = type;
	if (type == VARCHAR)
	{
		data._varchar = value;
	}
	else
	{
		data._int = value;
	}
}

template<typename T>
T Container::getData()
{
	if (_type == VARCHAR)
	{
		return data._varchar;
	}
	else
	{
		return data._int;
	}
}