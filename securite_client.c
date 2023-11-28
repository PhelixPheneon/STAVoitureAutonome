#include "securite_client.h"



//première fonction = celle pour decrypter les messages envoyés entre le client, le serveur et le gps
// fonction decrypte :




 // fonction pour encrypter les messages :
unsigned char encrypte_client(const unsigned char *plaintext, int plaintext_len) {
  RSA *public_key = NULL;
  FILE *fp =fopen("cle_public_serveur.key","rb");
  public_key = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
  fclose(fp);
  unsigned char *ciphertext = NULL;
  RSA_public_encrypt(plaintext_len,plaintext,ciphertext, public_key,RSA_PKCS1_PADDING);
  return *ciphertext;
}
//fonction pour decrypter les messages
unsigned char decrypte_client(const unsigned char *ciphertext, int ciphertext_len) {
  //chargement de la clé
  RSA *private_key = NULL;
  FILE *fp =fopen("cle_privee_voiture.key","rb");
  private_key = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
  fclose(fp);
  //decryptage
  unsigned char *plaintext;
  RSA_private_decrypt(ciphertext_len,ciphertext,plaintext,private_key ,RSA_PKCS1_PADDING);
  return *plaintext;
}
