from copy import deepcopy
from instruments.model import Model

class AudiometerModel(Model):
    def __init__(self, session):
        super().__init__(session)

    def is_valid(self, data) -> bool:
        if len(data) < 14:
            print("invalid length")
            return False

        return True

    def clear(self) -> None:
        self.metadata.clear()
        self.results.clear()

    def read_output(self) -> bool:
        self.clear()

    def set_manual_entry(self, data_entered: dict) -> bool:
        """
        Expects data entered to be a dictionary:
            [Left|Right][500 | 1000 | 2000 | 3000 | 4000 | 6000 | 8000]HzSpinBox: int
        """
        self.clear()

        for key, value in data_entered.items():
            clean_key = key.lower().replace('spinbox', '').replace('hz', '')

            test = clean_key[4:] if 'Left' in key else clean_key[5:]
            side = clean_key[:4] if 'Left' in key else clean_key[:5]
            db = value
            passed = value <= 40

            print(test, side, db, passed)

            self.results.append({
                'side': side,
                'error': "",
                'level': {
                    'units': 'db',
                    'value': value
                },
                'outcome': '',
                'pass': passed,
                'test': f'{test} Hz'
            })

        return True

    def get_response(self):
        return { 'results': deepcopy(self.results) }