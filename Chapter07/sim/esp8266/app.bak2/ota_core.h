


typedef void (*onInitCallback)();


class OtaCore {
	//
	
public:
	static bool init(onInitCallback cb);	
};