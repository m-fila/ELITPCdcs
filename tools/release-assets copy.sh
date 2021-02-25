#!/bin/bash

curl -k --header 'Content-Type: application/json' --header "PRIVATE-TOKEN: $CI_JOB_TOKEN" \
    --data '{ "name": \"$CI_COMMIT_TAG\", "tag_name": \"$CI_COMMIT_TAG\", "description": \"$(cat CHANEGLOG.md)\"}' \
    --request POST $CI_API_V4_URL/projects/$CI_PROJECT_ID/releases

curl --request POST \
    --header "PRIVATE-TOKEN: $CI_JOB_TOKEN" \
    --data name="ELITPC_DCS-v1.7.2-Linux.deb" \
    --data url="https://dracula.hep.fuw.edu.pl:8888/api/v4/projects/32/jobs/1236/artifacts/build/ELITPC_DCS-1.7.2-Linux.deb" \
    "https://dracula.hep.fuw.edu.pl:8888/api/v4/projects/32/releases/v1.7.2/assets/links"
