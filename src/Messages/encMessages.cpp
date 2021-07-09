#include "../../include/Messages/Client/enc.hpp"
#include "../../include/Messages/Server/enc.hpp"
#include "../../include/Utils/strinops.hpp"

/**
 * SECTION
 * Client encrypted messages
 */

// TODO setRandomness
int 
PlayerList::setRandomness()
{

}

size_t
PlayerList::serialize(unsigned char **data)
{
    size_t pos{0}, size{sizeof(struct PlayerList)};
    *data = new unsigned char[size];

    memcpy(*data, this->_randomness, RANDOMNESS_LENGTH_16);
    pos += RANDOMNESS_LENGTH_16;

    return size;
}

size_t
PlayerList::HtoN(unsigned char **data)
{
    size_t pos{0}, size{sizeof(struct PlayerList)};
    *data = new unsigned char[size];

    memcpy(*data, this->_randomness, RANDOMNESS_LENGTH_16);
    pos += RANDOMNESS_LENGTH_16;

    return size;
}

size_t
PlayerList::NtoH(unsigned char *data)
{
    size_t pos{0}, size{sizeof(struct PlayerList)};

    memcpy(this->_randomness, data, RANDOMNESS_LENGTH_16);
    pos += RANDOMNESS_LENGTH_16;

    return pos;
}

int
ChallengeRequest::setUsername(const char *user)
{   
    if(strlen(user) > 16)
    {
        std::cout << "Hello::setUsername() error: max lenght = 16 char";
        return -1; 
    }
    memcpy(this->_opp_username, user, USERNAME_LENGHT_16);
    this->_opp_username[USERNAME_LENGHT_16] = '\0';
    return 1;
}

size_t
ChallengeRequest::serialize(unsigned char **data)
{
    size_t pos{0}, size{sizeof(struct ChallengeRequest)};
    *data = new unsigned char[size];

    memcpy(*data, this->_opp_username, USERNAME_LENGHT_16);
    pos += USERNAME_LENGHT_16;

    return size;
}

size_t
ChallengeRequest::HtoN(unsigned char **data)
{
    size_t pos{0}, size{sizeof(struct ChallengeRequest)};
    *data = new unsigned char[size];

    memcpy(*data, this->_opp_username, USERNAME_LENGHT_16);
    pos += USERNAME_LENGHT_16;

    return size;
}

size_t
ChallengeRequest::NtoH(unsigned char *ser_buf)
{
    unsigned short int pos{0};

    memcpy(this->_opp_username, ser_buf, USERNAME_LENGHT_16);
    pos += USERNAME_LENGHT_16;

    return pos;
}

size_t
ChallengeResponse::serialize(unsigned char **data)
{
    size_t pos{0}, size{sizeof(struct ChallengeResponse)};
    *data = new unsigned char[size];

    memcpy(*data, &this->_answer, sizeof(this->_answer));
    pos += this->_answer;

    return size;
}

size_t
ChallengeResponse::HtoN(unsigned char **data)
{
    size_t pos{0}, size{sizeof(struct ChallengeResponse)};
    *data = new unsigned char[size];
    uint16_t answer{htons(this->_answer)};

    memcpy(*data, &answer, sizeof(answer));
    pos += sizeof(answer);

    return size;
}

size_t
ChallengeResponse::NtoH(unsigned char *ser_buf)
{
    size_t pos{0};
    uint16_t answer{0};

    memcpy(&answer, ser_buf, sizeof(answer));
    this->_answer = ntohs(answer);
    pos += sizeof(answer);

    return pos;
}

int
GameMove::setRandomness()
{
}

size_t
GameMove::serialize(unsigned char **data)
{
    size_t pos{0}, size{sizeof(struct GameMove)};
    *data = new unsigned char[size];

    memcpy(*data, &this->_column_num, sizeof(this->_column_num));
    pos += sizeof(this->_column_num);
    memcpy(*data + pos, this->_randomness, RANDOMNESS_LENGTH_16);
    pos += RANDOMNESS_LENGTH_16;

    return size;
}

size_t
GameMove::HtoN(unsigned char **data)
{
    size_t pos{0}, size{sizeof(struct GameMove)};
    uint16_t column_num{htons(this->_column_num)};
    *data = new unsigned char[size];

    memcpy(*data, &column_num, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(*data + pos, this->_randomness, RANDOMNESS_LENGTH_16);
    pos += RANDOMNESS_LENGTH_16;

    return size;
}

size_t
GameMove::NtoH(unsigned char *ser_buf)
{
    size_t pos{0};
    uint16_t column_num{0};

    memcpy(&column_num, ser_buf, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(this->_randomness, ser_buf + pos, RANDOMNESS_LENGTH_16);
    pos += RANDOMNESS_LENGTH_16;

    return pos;
}


int
GameEnd::setRandomness()
{
}

size_t
GameEnd::serialize(unsigned char **data)
{
    size_t pos{0}, size{sizeof(struct GameEnd)};
    *data = new unsigned char[size];

    memcpy(*data, &this->_signal, sizeof(this->_signal));
    pos += sizeof(this->_signal);
    memcpy(*data + pos, this->_randomness, RANDOMNESS_LENGTH_16);
    pos += RANDOMNESS_LENGTH_16;

    return size;
}

size_t
GameEnd::HtoN(unsigned char **data)
{
    size_t pos{0}, size{sizeof(struct GameEnd)};
    uint16_t signal{htons(this->_signal)};
    *data = new unsigned char[size];

    memcpy(*data, &signal, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(*data + pos, this->_randomness, RANDOMNESS_LENGTH_16);
    pos += RANDOMNESS_LENGTH_16;

    return size;
}

size_t
GameEnd::NtoH(unsigned char *ser_buf)
{
    size_t pos{0};
    uint16_t signal{0};

    memcpy(&signal, ser_buf, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(this->_randomness, ser_buf + pos, RANDOMNESS_LENGTH_16);
    pos += RANDOMNESS_LENGTH_16;

    return pos;
}

/**
 * SECTION
 * Server encrypted messages
 */
void
ClientList::addUser(char *name)
{
    User tmp;
    tmp.setName(name);
    this->users.emplace_back(tmp);
    this->_n_of_elem++;
}

void
ClientList::print()
{
    for(const auto& elem : this->users)
    {
        unsigned short int reminder = USERNAME_LENGHT_16 - strlen(elem._name);
        char format[reminder] = "";
        std::cout << 
            "|" << elem._name << format << "\t| " << "status: " << elem._status << "|" <<
            "\n----------------------------" <<
        std::endl;
    }
}

size_t
ClientList::serialize(unsigned char **data)
{
    size_t size{
        (this->_n_of_elem*USERNAME_LENGHT_16)+
        (this->_n_of_elem*sizeof(uint16_t))
    };
    size_t pos{0};
    *data = new unsigned char[size];

    memcpy(*data, &this->_n_of_elem, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    for(const auto& elem : this->users)
    {
        uint16_t status{htons(elem._status)};
        memcpy(*data + pos, elem._name, USERNAME_LENGHT_16);
        pos += USERNAME_LENGHT_16;
        memcpy(*data + pos, &status, sizeof(uint16_t));
        pos += sizeof(uint16_t);
    }
    return size;
} 

size_t
ClientList::HtoN(unsigned char **data)
{
    size_t size{
        (this->_n_of_elem*USERNAME_LENGHT_16)+
        (this->_n_of_elem*sizeof(uint16_t))
    };
    size_t pos{0};
    uint16_t n_of_elem{htons(this->_n_of_elem)};
    *data = new unsigned char[size];

    memcpy(*data, &n_of_elem, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    for(const auto& elem : this->users)
    {
        uint16_t status{htons(elem._status)};
        memcpy(*data + pos, elem._name, USERNAME_LENGHT_16);
        pos += USERNAME_LENGHT_16;
        memcpy(*data + pos, &status, sizeof(uint16_t));
        pos += sizeof(uint16_t);
    }
    return size;
} 

size_t
ClientList::NtoH(unsigned char *ser_buf)
{
    size_t pos{0};
    uint16_t n_of_elem{0};

    memcpy(&n_of_elem, ser_buf, sizeof(uint16_t));
    this->_n_of_elem = ntohs(n_of_elem);
    pos += sizeof(uint16_t);
    for (size_t i = 0; i < this->_n_of_elem; i++)
    {
        uint16_t status{0};
        User tmp;
        memcpy(tmp._name, ser_buf + pos, USERNAME_LENGHT_16);
        pos += USERNAME_LENGHT_16;
        memcpy(&status, ser_buf + pos, sizeof(uint16_t));
        tmp._status = ntohs(status);
        pos += sizeof(uint16_t);
        this->users.push_back(tmp);
    }

    return pos;
}