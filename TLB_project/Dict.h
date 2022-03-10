class Dict{
   int *key_arr;
   int *value_arr;
   int dict_size;
   
   public:
      Dict();
      Dict(int size);
      void update_entity(int index,int new_key,int new_value);
      bool has_key(int key);
      int get_by_key(int key);
      void free_lists();
};

Dict::Dict(){
   this->key_arr = NULL;
   this->value_arr = NULL;
   this->dict_size = 0;
}
Dict::Dict(int size){
   this->key_arr = (int*)calloc(size,sizeof(int));
   this->value_arr = (int*)calloc(size,sizeof(int));
   this->dict_size = size;
}

void Dict::update_entity(int index,int new_key,int new_value){
   //puts a new entity in the dictonary with some value and key
   this->key_arr[index] = new_key;
   this->value_arr[index] = new_value;
}

bool Dict::has_key(int key){
   //checks if the Dictonary has a given key
   for(int i = 0;i < this->dict_size;i++){
      if(this->key_arr[i] == key){
         return true;
      }
   }
   return false;
}

int Dict::get_by_key(int key){
   //looks through the dictonary and returns the value
   //associated with some key
   for(int i = 0;i < this->dict_size;i++){
      if(this->key_arr[i] == key){
         return this->value_arr[i];
      }
   }
   return -1;
}

void Dict::free_lists(){
   //cleenup method.
   free(this->key_arr);
   free(this->value_arr);
}