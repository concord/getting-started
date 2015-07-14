#/bin/bash --login
virtualenv .env
source .env/bin/activate
pip install -r requirements.txt
exec "$@"
