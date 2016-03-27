import os
import json

class SyncManager:
    """ Manages source files timestamps so
        parsing is done only if the file changed
    """

    def __init__(self, databasefile, sourcedir):
        self.databasefile = os.path.abspath(databasefile)
        self.rootpath = sourcedir

        if os.path.isfile(self.databasefile):
            with open(self.databasefile, 'r') as file:
                self.database = json.load(file)
        else:
            self.database = {}

    def fullpath(self, filename):
        return os.path.relpath(filename, self.rootpath)

    def save(self):
        with open(self.databasefile, 'w') as file:
            json.dump(self.database, file, indent = 4)

    def load_timestamp(self, sourcefile):
        return self.database[self.fullpath(sourcefile)]['timestamp']

    def get_file_timestamp(self, sourcefile):
        return os.path.getmtime(sourcefile)

    def save_timestamp(self, sourcefile):
        self.database[self.fullpath(sourcefile)]['timestamp'] = self.get_file_timestamp(sourcefile)

    def is_synced(self, filename):
        return self.fullpath(filename) in self.database and \
            self.get_file_timestamp(filename) <= self.load_timestamp(filename)

    def sync(self, filename, outputfile):
        key = self.fullpath(filename)

        if not key in self.database:
            self.database[key] = {}

        self.save_timestamp(filename)
        self.database[key]['output'] = outputfile

