#include "bucket.h"

class Stiqbucket : public Bucket {
public:
    Stiqbucket(string bucket_name, Baseplate *plate);
private:
    Briq *cast_data(byte *data, const int depth);
};
