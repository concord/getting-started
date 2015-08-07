import requests, os, json, re

def __memoize_no_args(fn):
    val = None
    memoized = False
    def new_fn():
        if not memoized:
            val = fn()
        return val
    return new_fn

@__memoize_no_args
def __circle_config():
    try:
        with open(os.path.expanduser("~/.circleci"), "r") as f:
            return json.loads(f.read())
    except IOError as e:
        if e.errno == 2:
            print("Error: Place your circleci token in a file at ~/.circleci")
        raise e
    except ValueError as e:
        print("Error: ~/.circleci must contain a JSON formatted blob")
        raise e

def __circle_get(*args, **kwargs):
    config = __circle_config()
    token = config["token"]
    template = "https://circleci.com/api/v1/%s?circle-token=%s"
    arg_str = "/".join(args)
    other_opts = map(lambda x: "%s=%s" % x, kwargs.iteritems())
    url = template % (arg_str, token)
    if len(other_opts):
        url += "&" + "&".join(other_opts)
    headers = None
    if kwargs.get("json"):
        kwargs.pop("json")
        headers = {"accept": "application/json"}
    return requests.get(url, headers=headers)

class ConcordException(Exception):
    def __init__(self, reason):
        self.reason = reason

    def __str__(self):
        return self.reason

def builds():
    config = __circle_config()
    builds = __circle_get("project", config["username"], config["project"],
            limit=10, filter="successful", json=True, branch="dev")
    if builds.ok:
        return filter(lambda x: x.get("has_artifacts"), builds.json())
    else:
        raise ConcordException(builds.text)

def artifact_for(build_num):
    config = __circle_config()
    res = __circle_get("project", config["username"], config["project"],
            str(build_num), "artifacts", json=True)
    if res.ok:
        matches = [x for x in res.json() if re.match(".+\.tar\.gz$", x["path"])]
        try:
            return next(iter(matches))["url"]
        except StopIteration:
            return None
    return None

def get_artifact(build_num, file_path=None):
    url = artifact_for(build_num)
    config = __circle_config()
    res = requests.get("%s?circle-token=%s" % (url, config["token"]))
    if file_path:
        with open(file_path, "w") as f:
            f.write(res.content)
    else:
        return res.content

