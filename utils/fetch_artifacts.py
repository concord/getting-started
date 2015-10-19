from concord_update import circle
import readline, tempfile, os

class ConcordException(Exception):
    def __init__(self, message):
        self.message = message

def pad_string(value, length=20):
    remaining = length - len(value)
    if remaining > 0:
        value += " " * remaining
    return value

def main():
    builds = circle.builds()
    commit_to_build = {}

    # print builds
    print("Builds found:")
    def get_build_info(build):
        details = build["all_commit_details"]
        if len(details):
            details = details[0]
        else:
            return None
        commit = details["commit"][0:6]
        name = pad_string(details["author_name"], 20)
        subject = details["subject"]
        return (commit, name, subject)

    for build in builds:
        info_tuple = get_build_info(build)
        if info_tuple is not None:
            commit, name, subject = info_tuple
            output = " | ".join([commit, name, subject])
            print(output)
            commit_to_build[commit] = build

    # set up commit completion
    class CommitCompleter(object):
        def __init__(self):
            self.commits = commit_to_build.keys()

        def complete(self, text, state):
            response = None
            if state == 0:
                if text:
                    self.matches = filter(lambda x: x.startswith(text),
                            self.commits)
                else:
                    self.matches = self.commits[:]

            try:
                response = self.matches[state]
            except IndexError:
                response = None

            return response

    readline.parse_and_bind("tab: complete")
    readline.set_completer(CommitCompleter().complete)

    # ask for a commit
    commit = raw_input("Select a commit: ")

    # error if does not exist
    if not commit in commit_to_build:
        raise ConcordException("Commit hash not found!")

    build = commit_to_build[commit]
    build_num = build["build_num"]

    print("Fetching build %d..." % build_num)

    tmp_dir = tempfile.mkdtemp()
    circle.get_artifact(build_num, os.path.join(tmp_dir, "concord.tar.gz"))

    old_path = os.path.realpath(os.curdir)
    os.chdir(tmp_dir)
    os.system("tar xzf concord.tar.gz")
    os.system("sudo mv concord/bolt_executor /usr/local/bin/")
    os.system("sudo mv concord/libbolt.so /usr/local/lib/")
    os.system("sudo mv concord/*.jar /usr/local/share/concord/")
    os.chdir(old_path)
    os.system("rm -rf %s" % tmp_dir)
    print("Updated!")

if __name__ == "__main__":
    main()
