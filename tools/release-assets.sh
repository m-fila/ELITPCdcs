#!/bin/bash

curl -k --silent --show-error --fail \
    --header "PRIVATE-TOKEN: $REPO_KEY" \
    --data name="ELITPC_DCS-$CI_COMMIT_TAG-Linux.deb" \
    --data url="$CI_API_V4_URL/projects/$CI_PROJECT_ID/jobs/artifacts/$CI_COMMIT_TAG/raw/build/ELITPC_DCS-$CI_COMMIT_TAG-Linux.deb?job=package" \
    --request POST "$CI_API_V4_URL/projects/$CI_PROJECT_ID/releases/$CI_COMMIT_TAG/assets/links"
