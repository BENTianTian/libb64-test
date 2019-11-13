#include <string>
#include <iostream>
//#include <sstream>
#define BUFFERSIZE 10000000
#include <b64/decode.h>
#include <b64/encode.h>
#include <cstring>

#define stringify( name ) # name

void test_string() {
  //std::ostringstream stream;
  //stream << "abc\0def\0ghi";
  std::string s;
  const char *c = "abc\0def\0ghi";
  std::cout << "c is " << c << std::endl;
  s.append(c,55);
  //s.append(c, 11);
  std::string s1;
  s1 += s;
  //while (1) {
  //  s = stream.str();
  //  std::cout << s << std::endl;
  //  if (s.empty()) {
  //    break;
  //  }
  //  s1.append(s);
  //}
  std::cout << "this is s1, length is " << s1.length() << std::endl;
  std::cout << "this is s1, size is " << s1.size() << std::endl;
  std::cout << s1 << std::endl;
  for (auto i : s1) {
    std::cout << i << std::endl;
  }
}

const char* state_name[] = {
  stringify( base64::step_A ),
  stringify( base64::step_B ),
  stringify( base64::step_C ),
  stringify( base64::step_D )
};

std::string de_state(base64::base64_decodestep e) {
  switch (e) {
    case base64::step_a: return std::string("a");
    case base64::step_b: return std::string("b");
    case base64::step_c: return std::string("c");
    case base64::step_d: return std::string("d");
  }
}

void test_decode() {
  base64::decoder d1;
  base64::decoder d2;
  base64::decoder d3;
  base64::decoder d4;

  base64::encoder e1;
  base64::encoder e2;
  base64::encoder e3;
  base64::encoder e4;

  char ori_str1[] = "a";
  char *ori_str2 = "ab";
  char *ori_str3 = "abc";
  char ori_str4[] = "abcd";

  char b64_str1[5];
  char b64_str2[6];
  char b64_str3[6];
  char b64_str4[10];

  char d64_str1[9];
  char d64_str2[9];
  char d64_str3[9];
  char d64_str4[9];

  char *c1 = b64_str1;
  char *c4 = b64_str4;
  e1.encode(ori_str1, 1, c1);
  // e1.encode_end(b64_str1);
  e2.encode(ori_str2, 2, b64_str2);
  e3.encode(ori_str3, 3, b64_str3);
  e4.encode(ori_str4, 4, c4);
  std::cout << "ori1 encode: " << b64_str1 << std::endl;
  std::cout << "ori4 encode: " << b64_str4 << std::endl;

  d1.decode(b64_str1, 4, d64_str1);
  d2.decode(b64_str2, 4, d64_str2);
  d3.decode(b64_str3, 4, d64_str3);
  d4.decode(b64_str4, 8, d64_str4);

  // std::cout << "ori1 state: " << state_name[d1._state.step] << std::endl;
}

void test_decode2() {
  base64::decoder d1;
  base64_init_decodestate(&(d1._state));
  // char ori_str1[] = "YQ=="; // state: 2
  // char ori_str1[] = "YWI="; // state:3
  char ori_str1[] = "YWJj"; // state:0
  // char ori_str1[] = "YWJjZA=="; // state:2
  char o_s1[4];

  d1.decode(ori_str1, sizeof(ori_str1), o_s1);

  std::cout << "after decode :" << o_s1 << std::endl;
  std::cout << "decode state :" << d1._state.step << std::endl;
}

std::string b64_decode(const char* ori_c) {
  int c_len = strlen(ori_c);
  int decode_len = c_len / 4 * 3;
  char decode_c[decode_len + 1] = "\0";
  char *c = decode_c;
  base64::decoder decoder(c_len);
  base64_init_decodestate(&(decoder._state));

  decoder.decode(ori_c, c_len, c);
  int empty_len = 0; // the number of "="
  switch (decoder._state.step) {
    case base64::step_a:
      empty_len = 0;
      break;
    case base64::step_c:
      empty_len = 2;
      break;
    case base64::step_d:
      empty_len = 1;
      break;
    default:
      break;
  }

  std::cout << "c len: " << c_len << std::endl;
  std::cout << "b64 is: " << ori_c << std::endl;
  std::cout << "actual len: " << strlen(decode_c) << std::endl;
  std::cout << "decode len: " << decode_len << std::endl;
  std::cout << "decode: " << decode_c << std::endl;
  return std::string(decode_c, decode_len - empty_len);
}

void test_encode1() {
  base64::encoder e1;
  base64_init_encodestate(&(e1._state));
  int o_len = 4;
  char ori_str1[] = "ab\0 ";
  // char o_s1[o_len * 4 / 3 + 2] = "\0";
  char o_s1[6] = "\0";
  char *c = o_s1;

  e1.encode(ori_str1, o_len, c);
  int tt = strlen(o_s1);
  e1.encode_end(c + tt);

  std::cout << "after encode :" << o_s1 << std::endl;
  std::cout << "encode state :" << e1._state.step << std::endl;
}

void test_b64_decode() {
  char a[] = "YWIAIA==";
  std::string s = b64_decode(a);
  std::cout << "decode is " << s << std::endl;
  std::cout << "decode len is " << s.length() << std::endl;
}

int main() {
  // test_decode2();
  test_encode1();
  test_b64_decode();
}
