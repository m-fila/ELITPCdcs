#!/bin/bash

curl -k --header "JOB-TOKEN: $CI_JOB_TOKEN" \
    --data name="ELITPC_DCS-$CI_COMMIT_TAG-Linux.deb" \
    --data url="$CI_API_V4_URL/projects/$CI_PROJECT_ID/jobs/artifacts/$CI_COMMIT_TAG/build/ELITPC_DCS-$CI_COMMIT_TAG-Linux.deb?job=package" \
    --request POST "$CI_API_V4_URL/projects/$CI_PROJECT_ID/releases/$CI_COMMIT_TAG/assets/links"
