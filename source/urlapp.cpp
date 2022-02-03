#include <iostream>
#include <string>
#include <curl/curl.h>

auto response = std::string {};

auto write_callback(void *data, size_t size, size_t count, void *) -> size_t {
  response.append(reinterpret_cast<char *>(data), size * count);
  return size * count;
}

void print_public_ip() {
  using std::cerr, std::cout;

  auto curl = curl_easy_init();

  if (!curl) {
    cerr << "curl_easy_init failed.\n";
    return;
  }

  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

  curl_easy_setopt(curl, CURLOPT_URL, "https://api.ipify.org/");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

  //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
  //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

  response = "";

  auto status = curl_easy_perform(curl);

  if (status == CURLE_OK)
    cout << "Public IP: " << response << '\n';
  else
    cerr << "curl_easy_perform failed: " << curl_easy_strerror(status) << '\n';

  curl_easy_cleanup(curl);
}

auto main() -> int {
  curl_global_init(CURL_GLOBAL_DEFAULT);

  print_public_ip();

  curl_global_cleanup();
  return 0;
}

