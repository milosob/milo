import json
import pathlib
import argparse

import src.cpp.test.primitive.aead_decrypt
import src.cpp.test.primitive.aead_encrypt
import src.cpp.test.primitive.cipher_decrypt
import src.cpp.test.primitive.cipher_encrypt
import src.cpp.test.primitive.codec_decode
import src.cpp.test.primitive.codec_encode
import src.cpp.test.primitive.hash
import src.cpp.test.primitive.kdf_hkdf
import src.cpp.test.primitive.kdf_pbkdf_2
import src.cpp.test.primitive.mac

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
        'test-primitive-aead-decrypt': src.cpp.test.primitive.aead_decrypt.gen,
        'test-primitive-aead-encrypt': src.cpp.test.primitive.aead_encrypt.gen,
        'test-primitive-cipher-decrypt': src.cpp.test.primitive.cipher_decrypt.gen,
        'test-primitive-cipher-encrypt': src.cpp.test.primitive.cipher_encrypt.gen,
        'test-primitive-codec-decode': src.cpp.test.primitive.codec_decode.gen,
        'test-primitive-codec-encode': src.cpp.test.primitive.codec_encode.gen,
        'test-primitive-hash': src.cpp.test.primitive.hash.gen,
        'test-primitive-kdf-hkdf': src.cpp.test.primitive.kdf_hkdf.gen,
        'test-primitive-kdf-pbkdf-2': src.cpp.test.primitive.kdf_pbkdf_2.gen,
        'test-primitive-mac': src.cpp.test.primitive.mac.gen
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
