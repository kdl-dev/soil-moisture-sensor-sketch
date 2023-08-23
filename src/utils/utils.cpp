#include "utils.h"

String validateSSID(String ssid) {
  if (ssid.length() < 2) {
    return "The enforced minimum length of a SSID is 2 characters.";
  } else if (ssid.length() > 32) {
    return "The enforced maximum length of a SSID is 32 characters.";
  }

  int asciiNumber;
  for (int i = 0; i < ssid.length(); i++) {
    asciiNumber = int(ssid[i]);

    // [0-9]
    if (asciiNumber >= 48 && asciiNumber <= 57) {
      continue;
    }

    // [A-Z]
    if (asciiNumber >= 65 && asciiNumber <= 90) {
      continue;
    }

    // [a-z]
    if (asciiNumber >= 97 && asciiNumber <= 122) {
      continue;
    }

    // - _ 'SPACE'
    if (asciiNumber == 45 || asciiNumber == 95 || asciiNumber == 32) {
      continue;
    }

    return "Allowed characters for SSID: Latin letters of both cases (a-z A-Z), numbers (0-9), dash (-), underscore (_) and space.";
  }

  return "";
}

String validatePass(String pass) {
  if (pass.length() < 8) {
    return "The enforced minimum length of a WPA password is 8 characters.";
  } else if (pass.length() > 63) {
    return "The enforced maximum length of a WPA password is 63 characters.";
  }

  int asciiNumber;
  String specCharacters(" \"!\"#$%&'()*+,—./:;<=>?@[\\]^_`{|}~");
  for (int i = 0; i < pass.length(); i++) {
    asciiNumber = int(pass[i]);

    // [0-9]
    if (asciiNumber >= 48 && asciiNumber <= 57) {
      continue;
    }

    // [A-Z]
    if (asciiNumber >= 65 && asciiNumber <= 90) {
      continue;
    }

    // [a-z]
    if (asciiNumber >= 97 && asciiNumber <= 122) {
      continue;
    }

    if (specCharacters.indexOf(pass[i]) != -1) {
      continue;
    }

    return "Allowed characters for password: Latin letters of both cases (a-z A-Z), numbers (0-9), and special characters (\"!\"#$%&'()*+,—./:;<=>?@[\\]^_`{|}~ and space)";
  }

  return "";
}
