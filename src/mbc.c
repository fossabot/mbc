#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>
#include <libmbc.h>

#define print_version()  fprintf(stderr, VERSION_INFO)
#define print_usage()    fprintf(stderr, "Usage: %s %s\n", CLI_NAME, USAGE_INFO)
#define print_help()     print_version(); fprintf(stderr, "\n%s\n", SHORT_DESC); print_usage(); fprintf(stderr, "\n%s", HELP_MSG)

#define MBC_VERSION "0.1"

static const char* VERSION_INFO = "mbc " MBC_VERSION "\nCopyright (c) 2017 Matteo Bernardini & Marco Bonelli.\n";
static const char* USAGE_INFO = "[-xuvh] (-e | -d) -k <key>";
static const char* SHORT_DESC =
	"mbc is a quick tool for encoding/decoding data via stdio using libmbc,\n"
	"a C implementation of the Mattyw & MeBeiM symmetric encryption algorithm.\n"
	"Source code available at https://github.com/mttbernardini/mbc.\n";
static const char* HELP_MSG =
	"Options:\n"
	" -e        Encodes data from stdio and outputs it to stdout.\n"
	" -d        Decodes data from stdio and outputs it to stdout.\n"
	" -k <key>  Sets the key for the encryption.\n"
	" -x        When encoding takes raw input from stdin and outputs encoded\n"
	"           data as an hexadecimal string to stdout. When decoding takes\n"
	"           an hexadecimal string representing encoded data from stdin\n"
	"           and outputs raw decoded data to stdout.\n"
	" -u        When using -x, output uppercase characters.\n"
	" -v        Shows program version and exits.\n"
	" -h        Shows this help message and exits.\n";

static const size_t RAW_CHUNK_SIZE = 32 << 20;  // TODO: make chunk size user editable,
static const size_t HEX_CHUNK_SIZE = 64 << 20;  //       but how do we deal with linearity?

static char* CLI_NAME;

void core(bool, bool, bool, char*);

int main(int argc, char* argv[]) {
	enum {NONE, ENCODE, DECODE, INVALID} mode;
	char opt, *key;
	bool hex, upper;

	CLI_NAME = argv[0];
	key      = NULL;
	hex      = false;
	upper    = false;
	mode     = NONE;

	while ((opt = getopt(argc, argv, "edk:xuvh")) != -1) {
		switch (opt) {
			case 'e':
				mode = (mode == NONE ? ENCODE : INVALID);
				break;

			case 'd':
				mode = (mode == NONE ? DECODE : INVALID);
				break;

			case 'k':
				key = optarg;
				break;

			case 'x':
				hex = true;
				break;

			case 'u':
				upper = true;
				break;

			case 'h':
				print_help();
				return 0;

			case 'v':
				print_version();
				return 0;

			default:
				print_usage();
				return 1;
		}
	}

	if (mode == NONE || mode == INVALID || key == NULL) {
		fprintf(stderr, "%s: please set a valid mode (-d OR -e) and a key (-k).\n", CLI_NAME);
		print_usage();
		return 1;
	}

	core(mode == ENCODE, hex, upper, key);

	return 0;
}

void core(bool enc_mode, bool hex_mode, bool uppercase, char* user_key) {
	uint8_t *buffer_in_raw, *buffer_out_raw;
	char *buffer_in_hex, *buffer_out_hex;
	size_t user_key_size, bytes_read, bytes_to_write;

	user_key_size = strlen(user_key);
	mbc_set_user_key((uint8_t*) user_key, user_key_size);
	atexit(mbc_free);

	if (hex_mode) {
		if (enc_mode) {

			buffer_in_raw = malloc(RAW_CHUNK_SIZE);
			if (buffer_in_raw == NULL)
				exit(1);

			while ((bytes_read = fread(buffer_in_raw, 1, RAW_CHUNK_SIZE, stdin))) {
				buffer_out_hex = mbc_encode_to_hex(buffer_in_raw, bytes_read, uppercase);
				if (buffer_out_hex == NULL) {
					free(buffer_in_raw);
					exit(1);
				}

				fwrite(buffer_out_hex, 1, bytes_read * 2, stdout);
				free(buffer_out_hex);
			}

			free(buffer_in_raw);
		} else {

			buffer_in_hex = malloc(HEX_CHUNK_SIZE + 1);
			if (buffer_in_hex == NULL)
				exit(1);

			while ((bytes_read = fread(buffer_in_hex, 1, HEX_CHUNK_SIZE, stdin))) {
				buffer_in_hex[bytes_read] = '\0';
				buffer_out_raw = mbc_decode_from_hex(buffer_in_hex, &bytes_to_write);
				if (buffer_out_raw == NULL) {
					free(buffer_in_hex);
					exit(1);
				}

				fwrite(buffer_out_raw, 1, bytes_to_write, stdout);
				free(buffer_out_raw);
			}

			free(buffer_in_hex);
		}
	} else {

		buffer_in_raw = malloc(RAW_CHUNK_SIZE);
		if (buffer_in_raw == NULL)
			exit(1);

		if (enc_mode) {
			while ((bytes_read = fread(buffer_in_raw, 1, RAW_CHUNK_SIZE, stdin))) {
				mbc_encode_inplace(buffer_in_raw, bytes_read);
				fwrite(buffer_in_raw, 1, bytes_read, stdout);
			}
		} else {
			while ((bytes_read = fread(buffer_in_raw, 1, RAW_CHUNK_SIZE, stdin))) {
				mbc_decode_inplace(buffer_in_raw, bytes_read);
				fwrite(buffer_in_raw, 1, bytes_read, stdout);
			}
		}

		free(buffer_in_raw);
	}
}
