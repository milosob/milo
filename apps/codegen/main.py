import json
import pathlib
import argparse

import src.cpp.test.aead_decrypt
import src.cpp.test.aead_encrypt
import src.cpp.test.cipher_decrypt
import src.cpp.test.cipher_encrypt
import src.cpp.test.codec_decode
import src.cpp.test.codec_encode
import src.cpp.test.hash
import src.cpp.test.kdf_hkdf
import src.cpp.test.kdf_pbkdf_2
import src.cpp.test.mac

parser: argparse.ArgumentParser
parser = argparse.ArgumentParser(
    formatter_class=argparse.RawDescriptionHelpFormatter,
    description='''
    milo source code generation tool.\
    '''
)
parser.add_argument(
    '--config-path',
    help='config path',
    type=pathlib.Path,
    required=True
)
parser.add_argument(
    '--output-path',
    help='output path',
    type=pathlib.Path,
    required=True
)


def main(
) -> None:
    args = parser.parse_args()

    config_path: pathlib.Path
    config_path = args.config_path

    output_path: pathlib.Path
    output_path = args.output_path

    assert config_path.is_file()

    config: dict
    config = json.load(open(config_path))

    assert 'scheme' in config
    assert 'payload' in config

    callbacks: dict
    callbacks = {
        'test-aead-decrypt': src.cpp.test.aead_decrypt.gen,
        'test-aead-encrypt': src.cpp.test.aead_encrypt.gen,
        'test-cipher-decrypt': src.cpp.test.cipher_decrypt.gen,
        'test-cipher-encrypt': src.cpp.test.cipher_encrypt.gen,
        'test-codec-decode': src.cpp.test.codec_decode.gen,
        'test-codec-encode': src.cpp.test.codec_encode.gen,
        'test-hash': src.cpp.test.hash.gen,
        'test-kdf-hkdf': src.cpp.test.kdf_hkdf.gen,
        'test-kdf-pbkdf-2': src.cpp.test.kdf_pbkdf_2.gen,
        'test-mac': src.cpp.test.mac.gen
    }

    scheme: str
    scheme = config['scheme']

    payload: dict
    payload = config['payload']

    code: str
    code = callbacks[scheme](
        payload
    )

    with open(output_path, 'w') as o_file:
        o_file.write(code)
