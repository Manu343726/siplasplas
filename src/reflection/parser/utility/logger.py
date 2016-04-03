from colorama import Fore, Back, Style
import os

def percent_string(step, total):
    percent = 100 * step / total

    if percent < 10:
        return '  {}%'.format(percent)
    elif percent < 100:
        return ' {}%'.format(percent)
    else:
        return '100%'

class PathFormatter:
    def __init__(self, sourcedir):
        self.sourcedir = sourcedir

    def format(self, path, color = Fore.WHITE, format = Style.NORMAL):
        return '{}{}{}'.format(color, os.path.relpath(path, self.sourcedir), Fore.RESET)


class Logger:
    def __init__(self, color, totalsteps = 0, tab = ''):
        self.totalsteps = totalsteps
        self.currentstep = 1

        if totalsteps > 0:
            self.step_header = lambda step: '[{}{}{}] '.format(Fore.BLUE + Style.BRIGHT, percent_string(step, totalsteps), Fore.RESET + Style.RESET_ALL)
        else:
            self.step_header = lambda step: tab + color + '  -> ' + Fore.RESET

        self.start_header = tab + color + '==> ' + Fore.RESET
        self.info_header = tab + ' - '

    def start(self, message, steps = -1):
        if message >= 0:
            self.totalsteps = steps

        self.currentstep = 1

        print ''
        print self.start_header + message

    def step(self, message):
        print self.step_header(self.currentstep) + message
        self.currentstep += 1

    def info(self, message):
        print self.info_header + message

class DevNullLogger:
    def start(self, message, steps = -1):
        return

    def step(self, message):
        return

    def info(self, message):
        return


class GlobalLogger:
    _okLogger = Logger(Fore.GREEN)
    _errorLogger = Logger(Fore.RED)
    _warnLogger = Logger(Fore.YELLOW)
    _infoLogger = Logger(Fore.BLUE)
    _normalLogger = Logger(Fore.WHITE)
    _devNullLogger = DevNullLogger()
    _enabled = False

    @staticmethod
    def enabled(enabled):
        GlobalLogger._enabled = enabled

    @staticmethod
    def ok():
        return GlobalLogger._okLogger if GlobalLogger._enabled else GlobalLogger._devNullLogger

    @staticmethod
    def error():
        return GlobalLogger._errorLogger

    @staticmethod
    def warning():
        return GlobalLogger._warnLogger if GlobalLogger._enabled else GlobalLogger._devNullLogger

    @staticmethod
    def info():
        return GlobalLogger._infoLogger if GlobalLogger._enabled else GlobalLogger._devNullLogger

    @staticmethod
    def normal():
        return GlobalLogger._normalLogger if GlobalLogger._enabled else GlobalLogger._devNullLogger


