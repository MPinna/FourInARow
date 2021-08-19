/** DESCRIPTION:
 * This file it is used to implement serialization / deserialization class member and other utils functionalities related to each struct for authentication messages only (both for client side and server side)
 */
#include "../../include/Messages/Client/auth.hpp"
#include "../../include/Messages/Server/auth.hpp"
#include "../../include/Utils/utils.hpp"

/**
 * SECTION: 
 * Client authentication messages 
 */
int
ClientHello::setUsername(const char *user)
{   
    if(strlen(user) > 16)
    {
        std::cout << "ClientHello::setUsername() error: max lenght = 16 char";
        return -1;
    }
    memcpy(this->_username, user, strlen(user));
    this->_username[USERNAME_LENGHT_16] = '\0';
    return 1;
}

int
ClientHello::getType()
{
    return CLIENT_HELLO;
}

size_t
ClientHello::getSize()
{
    return sizeof(struct ClientHello);
}

size_t
ClientHello::serialize(unsigned char **data)
{
    unsigned short int pos{0};
    size_t size{sizeof(struct ClientHello)};

    memcpy(*data, this->_username, USERNAME_LENGHT_16);
    pos += USERNAME_LENGHT_16;
    memcpy(*data + pos, &this->_port_number, sizeof(this->_port_number));
    pos += sizeof(this->_port_number);
    memcpy(*data + pos, &this->_nonce, sizeof(this->_nonce));
    pos += sizeof(this->_nonce);

    return size;
}

size_t
ClientHello::HtoN(unsigned char ** to_ser)
{
    uint16_t port_num{htons(this->_port_number)};
    uint32_t nonce{htonl(this->_nonce)};
    unsigned short int pos{0};
    size_t size{USERNAME_LENGHT_16 + sizeof(this->_port_number) + sizeof(this->_nonce)};
    *to_ser = new unsigned char[size];

    memcpy(*to_ser + pos, this->_username, USERNAME_LENGHT_16);
    pos += USERNAME_LENGHT_16;
    memcpy(*to_ser + pos, &port_num, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(*to_ser + pos, &nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);

    return size;
}

size_t
ClientHello::NtoH(unsigned char *ser_buf)
{
    size_t pos{0};
    uint16_t port_num{0};
    uint32_t nonce{0};
    
    memcpy(this->_username, ser_buf, USERNAME_LENGHT_16);
    pos += USERNAME_LENGHT_16;
    memcpy(&port_num, ser_buf + pos, sizeof(uint16_t));
    this->_port_number = ntohs(port_num);
    pos += sizeof(uint16_t);
    memcpy(&nonce, ser_buf + pos, sizeof(uint32_t));
    this->_nonce = ntohl(nonce);
    pos += sizeof(uint32_t);

    return pos;
}

void
ClientHello::print()
{
    std::cout << 
        "Username:    "   << this->_username <<
        "\nPort number: " << this->_port_number <<
        "\nNonce:       " << this->_nonce <<
    std::endl;
}

int
ClientResponse::getType()
{
    return CLIENT_RESPONSE;
}

size_t
ClientResponse::getSize()
{
    return (
        sizeof(this->_opp_nonce) +
        this->dh_key.getSize()
    );
}

size_t
ClientResponse::serialize(unsigned char **data)
{
    uint32_t opp_nonce{htonl(this->_opp_nonce)};
    size_t size{this->getSize()}, pos{0};
    *data = new unsigned char[size];

    memcpy(*data, &opp_nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    this->dh_key.serialize(*data + pos);

    return size;
}

size_t
ClientResponse::HtoN(unsigned char **data)
{
    uint32_t opp_nonce{htons(this->_opp_nonce)};
    size_t pos{0}, size{this->getSize()};
    *data = new unsigned char[size];

    memcpy(*data + pos, &opp_nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    this->dh_key.HtoN(*data + pos);

    return size;
}

size_t
ClientResponse::NtoH(unsigned char *ser_data)
{
    size_t pos{0};
    uint32_t nonce{0};
    
    memcpy(&nonce, ser_data, sizeof(uint32_t));
    this->_opp_nonce = ntohl(nonce);
    pos += sizeof(uint32_t);
    this->dh_key.NtoH(ser_data+pos);

    return pos;
}

void
ClientResponse::print()
{
    std::cout <<
        "Opponent nonce: " << this->_opp_nonce << 
    std::endl;
    this->dh_key.print();
}

int
ChallengeRequest::getType()
{
    return 1; //FIXME
}

size_t
ChallengeRequest::getSize()
{
    return (
        sizeof(this->_nonce) + 
        sizeof(this->_opp_nonce) + 
        this->dh_key.getSize()
    );
}

size_t
ChallengeRequest::serialize(unsigned char **data)
{
    size_t pos{0}, size{this->getSize()};
    *data = new unsigned char[size];

    memcpy(*data, &this->_nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(*data, &this->_opp_nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    this->dh_key.serialize(*data + pos);

    return size;
}

size_t
ChallengeRequest::HtoN(unsigned char **data)
{
    uint32_t nonce{htonl(this->_nonce)}, opp_nonce{htonl(this->_opp_nonce)};
    size_t pos{0}, size{this->getSize()};
    *data = new unsigned char[size];

    memcpy(*data, &nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(*data, &opp_nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    this->dh_key.HtoN(*data + pos);

    return size;
}

size_t
ChallengeRequest::NtoH(unsigned char *ser_data)
{
    size_t pos{0};
    uint32_t nonce{0}, opp_nonce{0};

    memcpy(&nonce, ser_data, sizeof(nonce));
    this->_nonce = ntohl(nonce);
    pos += sizeof(nonce);
    memcpy(&opp_nonce, ser_data + pos, sizeof(opp_nonce));
    this->_opp_nonce = ntohl(opp_nonce);
    pos += sizeof(opp_nonce);
    this->dh_key.NtoH(ser_data + pos);

    return pos;
}

void
ChallengeRequest::print()
{
    std::cout <<
        "Nonce: " << this->_nonce <<
        "Opponent Nonce: " << this->_opp_nonce <<
    std::endl;
    this->dh_key.print();
}

int
ChallengeResponse::getType()
{
    return CHALLENGE_RESPONSE;
}

size_t
ChallengeResponse::getSize()
{
    return (
        sizeof(this->_opp_nonce) +
        sizeof(this->_signal) +
        this->dh_key.getSize()
    );
}

size_t
ChallengeResponse::serialize(unsigned char **data)
{
    size_t pos{0}, size{this->getSize()};
    *data = new unsigned char[size];

    memcpy(*data, &this->_opp_nonce, sizeof(_opp_nonce));
    pos += sizeof(_opp_nonce);
    memcpy(*data + pos, &this->_signal, sizeof(_signal));
    pos += sizeof(_signal);
    this->dh_key.serialize(*data + pos);

    return size;
}

size_t
ChallengeResponse::HtoN(unsigned char **data)
{
    uint32_t opp_nonce{htonl(this->_opp_nonce)};
    uint16_t signal{htons(this->_signal)};
    size_t pos{0}, size{this->getSize()};
    *data = new unsigned char[size];

    memcpy(*data, &opp_nonce, sizeof(opp_nonce));
    pos += sizeof(opp_nonce);
    memcpy(*data + pos, &signal, sizeof(signal));
    pos += sizeof(signal);
    this->dh_key.HtoN(*data + pos);

    return size;
}

size_t
ChallengeResponse::NtoH(unsigned char *ser_data)
{
    size_t pos{0};
    uint32_t opp_nonce{0};
    uint16_t signal{0};

    memcpy(&opp_nonce, ser_data, sizeof(opp_nonce));
    this->_opp_nonce = ntohl(opp_nonce);
    pos += sizeof(opp_nonce);
    memcpy(&signal, ser_data + pos, sizeof(signal));
    this->_signal = ntohl(signal);
    pos += sizeof(opp_nonce);
    this->dh_key.NtoH(ser_data + pos);

    return pos;
}

void
ChallengeResponse::print()
{
    std::cout << 
        "Opponent Nonce: " << this->_opp_nonce <<
        "Signal: " << this->_signal << 
    std::endl;
    this->dh_key.print();
}

/**
 * SECTION
 * Server authentication messages 
 */
size_t
ServerCertificate::setCert(unsigned char * cert)
{   
    size_t size{strlen((char *)cert)};
    this->_certificate = new unsigned char[size + 1];
    memcpy(this->_certificate, cert + '\0', size+1);
    this->_lenght = size; 

    return size;
}

size_t
ServerCertificate::getSize()
{
    return sizeof(this->_lenght) + this->_lenght;
}

size_t
ServerCertificate::serialize(unsigned char **data)
{
    size_t pos{0}, size{this->getSize()};
    *data = new unsigned char[size];

    memcpy(*data, &this->_lenght, sizeof(this->_lenght));
    pos += sizeof(this->_lenght);
    memcpy(*data + pos, this->_certificate, this->_lenght);
    pos += this->_lenght;

    return pos;
}

size_t
ServerCertificate::HtoN(unsigned char **data)
{
    size_t pos{0}, size{this->getSize()};
    uint16_t lenght{htons(this->_lenght)};
    *data = new unsigned char[size];

    memcpy(*data, &lenght, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(data + pos, this->_certificate, this->_lenght);

    return size;
}

size_t
ServerCertificate::NtoH(unsigned char *ser_buf)
{
    short int pos{0};
    uint16_t lenght{0};

    memcpy(&lenght, ser_buf, sizeof(uint16_t));
    this->_lenght = ntohs(lenght);
    pos += sizeof(uint16_t);
    this->setCert(ser_buf + pos);

    return pos;
}

int
ServerResponse::getType()
{
    return 1; // FIXME
}

size_t
ServerResponse::getSize()
{
    return (
        sizeof(this->_opp_nonce) + 
        sizeof(this->_dh_param_g) +
        sizeof(this->_dh_param_p) +
        this->dh_key.getSize()
    );
}

size_t
ServerResponse::serialize(unsigned char **data)
{
    size_t pos{0}, size{this->getSize()};
    *data = new unsigned char[size];

    memcpy(*data, &this->_opp_nonce, sizeof(this->_opp_nonce));
    pos += sizeof(this->_opp_nonce);
    memcpy(*data + pos, &this->_dh_param_p, sizeof(this->_dh_param_p));
    pos += sizeof(this->_dh_param_p);
    memcpy(*data + pos, &this->_dh_param_g, sizeof(this->_dh_param_g));
    pos += sizeof(this->_dh_param_g);
    this->dh_key.serialize(*data + pos);

    return size;
}

size_t
ServerResponse::HtoN(unsigned char **data)
{
    size_t pos{0}, size{this->getSize()};
    uint32_t opp_nonce{htonl(this->_opp_nonce)}, param_g{htonl(this->_dh_param_g)}, param_p{htonl(this->_dh_param_p)};
    *data = new unsigned char[size];

    memcpy(*data, &opp_nonce, sizeof(opp_nonce));
    pos += sizeof(opp_nonce);
    memcpy(*data + pos, &param_p, sizeof(param_p));
    pos += sizeof(param_p);
    memcpy(*data + pos, &param_g, sizeof(param_g));
    pos += sizeof(param_g);
    this->dh_key.serialize(*data + pos);

    return size;
}

size_t
ServerResponse::NtoH(unsigned char *data)
{
    size_t pos{0};
    uint32_t opp_nonce{0}, param_g{0}, param_p{0};

    memcpy(&opp_nonce, data, sizeof(opp_nonce));
    this->_opp_nonce = ntohl(opp_nonce);
    pos += sizeof(opp_nonce);
    memcpy(&param_p, data + pos, sizeof(param_p));
    this->_dh_param_p = ntohl(param_p);
    pos += sizeof(param_p);
    memcpy(&param_g, data + pos, sizeof(param_g));
    this->_dh_param_g = ntohl(param_g);
    pos += sizeof(param_g);
    this->dh_key.NtoH(data + pos);

    return pos;
}

// TODO implementation of confirmation structure