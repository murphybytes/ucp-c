#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/cryptlib.h>
#include "encryption_service.hpp"

using CryptoPP::AutoSeededRandomPool;
using CryptoPP::Rijndael_Info;


namespace ucp {

  void encryption_service::generate_shared_secret( byte_string& shared_secret ) const {
    AutoSeededRandomPool generator;
    Rijndael_Info aes_params;
    unsigned char  key[ aes_params.DEFAULT_KEYLENGTH ];
    generator.GenerateBlock( key, aes_params.DEFAULT_KEYLENGTH );
    shared_secret.append(  key, aes_params.DEFAULT_KEYLENGTH );
    
  }

}
