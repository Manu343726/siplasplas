#!/usr/bin/env python2
#
# This script checks if a commit message includes a corresponding github issue number
# and if that issue is open. Uses pygithub (https://github.com/PyGithub/PyGithub) for github
# API interaction.
#
# Commit messages must follow the following format:
#
#     [#<issue number>] <brief>
#     <commit message body>
#
# To use the script in your proyect, replace the value of the
# GITHUB_REPOSITORY constant bellow with the name of your Github
# repository ("username/repository").
#
# The script is not designed to be used as a git hook directly,
# but to be called from a hook. The hook must pass the commit message
# brief (The first line of the message, $(git log -1 --pretty=%s <commit sha>))
#
# For example:
#
#    python2 check-issue-number.py "[#42] The answer to the universe and Everything"
#
# The script returns:
#  - 0 if the commit message includes an issue number, the issue exists,
#    and it's open.
#  - 1 if the commit message doesn't include an issue number.
#  - 2 if the commit message includes an issue number but the repository
#    has no issue with that number.
#  - 3 if the commit message includes an issue number but the issue is
#    closed.

from github import Github
from github.GithubException import UnknownObjectException
import sys
import re

GITHUB_REPOSITORY = 'manu343726/siplasplas'

commit_brief = sys.argv[1] # git log -1 --pretty=%s <your commit sha>
github = Github()
repo = github.get_repo(GITHUB_REPOSITORY)
search = re.search('\[\#([0-9]+)\] (.+)', commit_brief)

if search is not None:
    commit_issue_number = int(search.group(1))
    commit_brief_message = search.group(2)

    try:
        issue = repo.get_issue(commit_issue_number)
    except UnknownObjectException:
        issue = None

    if issue is not None:
        if issue.state == 'closed':
            print '    Error: issue #{} "{}" is closed'.format(issue.number, issue.title)
            exit(3)
        else:
            exit()
    else:
        print '    Error: {} has no issue #{}'.format(GITHUB_REPOSITORY, commit_issue_number)
        exit(2)
else:
    print '    Error: Commit messages must follow format "[#<issue id>] <brief>"'
    exit(1)
