/**
 * Created by Ozgur Cimen on 19-Nov-17.
 */

#include <iostream>
#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/cognito-idp/model/AdminInitiateAuthRequest.h>

#include <aws\cognito-identity\model\GetOpenIdTokenForDeveloperIdentityRequest.h>
#include <aws\cognito-identity\CognitoIdentityClient.h>
#include <aws\core\auth\AWSCredentialsProvider.h>
#include <aws/cognito-idp/CognitoIdentityProviderClient.h>
#include <aws/cognito-idp/model/DescribeUserPoolClientRequest.h>
#include <aws/cognito-idp/model/CreateUserPoolClientRequest.h>
#include <aws/cognito-idp/model/SignUpRequest.h>
#include <aws/cognito-idp/model/InitiateAuthRequest.h>
#include <aws/cognito-idp/model/RespondToAuthChallengeRequest.h>
#include <aws/cognito-idp/model/DescribeUserPoolRequest.h>
#include <aws/cognito-identity/model/GetOpenIdTokenRequest.h>
#include <aws/cognito-identity/model/GetIdRequest.h>
#include <aws/cognito-identity/model/CreateIdentityPoolRequest.h>
#include <aws/cognito-identity/model/DescribeIdentityPoolRequest.h>
#include <aws/cognito-idp/model/ChangePasswordRequest.h>
#include <aws/cognito-idp/model/ForgotPasswordRequest.h>
#include <aws/cognito-idp/model/ConfirmForgotPasswordRequest.h>
#include <aws/cognito-idp/model/GetUserRequest.h>
#include <aws/cognito-identity/model/GetCredentialsForIdentityRequest.h>

#define CLIENT_ID "533slgvh4oome8lh"
#define IDENTITY_POOLID "eu-central-1:71-bb76-fc4bad62382c"
#define USER_POOLID "eu-cen7UCVSequ"

using namespace Aws::Auth;
using namespace Aws::CognitoIdentity;
using namespace Aws::CognitoIdentity::Model;
using namespace Aws::CognitoIdentityProvider;
using namespace Aws::CognitoIdentityProvider::Model;


int main() {
    std::cout << "Koc University AWS SDK Cognito Authentication Test Sample!" << std::endl;

    const char* ALLOCATION_TAG;

    typedef Aws::Utils::Outcome<GetUICustomizationResult, Aws::Client::AWSError<CognitoIdentityProviderErrors> > salla;


    Aws::SDKOptions options;
    Aws::InitAPI(options);
    {
        std::cout << "AWS SDK Initialized"<< std::endl;

        std::shared_ptr<Aws::Auth::AnonymousAWSCredentialsProvider> credentialProvider;

        Aws::Client::ClientConfiguration clientConfig;
        clientConfig.region = Aws::Region::EU_CENTRAL_1;
        clientConfig.scheme = Aws::Http::Scheme::HTTPS;
        clientConfig.connectTimeoutMs = 30000;
        clientConfig.requestTimeoutMs = 30000;

		std::cout << "Configuration has been set" << std::endl;

        CognitoIdentityProviderClient client = CognitoIdentityProviderClient(clientConfig);
		
		std::cout << "Client has been created" << std::endl;
		     				
        //User pool description
        DescribeUserPoolClientRequest describeUserPoolClientRequest;
        describeUserPoolClientRequest.WithUserPoolId(USER_POOLID).WithClientId(CLIENT_ID);
        DescribeUserPoolClientOutcome describeUserPoolClientOutcome = client.DescribeUserPoolClient(describeUserPoolClientRequest);

		if (describeUserPoolClientOutcome.IsSuccess()) {
			std::cout << "describeUserPoolClientOutcome is succees" << std::endl;

			
			Aws::Map<Aws::String, Aws::String> authParameters;						
			authParameters.insert(std::pair<Aws::String, Aws::String>("USERNAME", ""));
			authParameters.insert(std::pair<Aws::String, Aws::String>("PASSWORD", "!"));
			
			/*"AuthFlow":"ADMIN_NO_SRP_AUTH",
				"AuthParameters" : {
				"USERNAME":"<username>",
					"PASSWORD" : "<password>"
			},
				"ClientId" : "<clientId>",
					"UserPoolId" : "<userPoolId>"*/
						

			//SRP gerektirmeden
			//Without srp 
			AdminInitiateAuthRequest adminInitiateAuthRequest;

			adminInitiateAuthRequest.SetAuthFlow(Aws::CognitoIdentityProvider::Model::AuthFlowType::ADMIN_NO_SRP_AUTH);
			adminInitiateAuthRequest.SetClientId(CLIENT_ID);
			adminInitiateAuthRequest.SetUserPoolId(USER_POOLID);
			adminInitiateAuthRequest.SetAuthParameters(authParameters);			
			AdminInitiateAuthOutcome initiateAuthOutcome = client.AdminInitiateAuth(adminInitiateAuthRequest);
			

			//SRP gerektirir burası biraz iş sonra yapıcam 
			//todo with SRP 
			/*InitiateAuthRequest initiateAuthRequest;
			initiateAuthRequest.SetAuthFlow(Aws::CognitoIdentityProvider::Model::AuthFlowType::USER_SRP_AUTH);		
			initiateAuthRequest.SetClientId(CLIENT_ID);			
			initiateAuthRequest.SetAuthParameters(authParameters);
			InitiateAuthOutcome initiateAuthOutcome = client.InitiateAuth(initiateAuthRequest);*/


			if (initiateAuthOutcome.IsSuccess()) {
				std::cout << "initiateAuthOutcome is Success"<< std::endl;
				
				RespondToAuthChallengeRequest respondToAuthChallengeRequest;
				respondToAuthChallengeRequest.SetChallengeName(initiateAuthOutcome.GetResultWithOwnership().GetChallengeName());
				//  respondToAuthChallengeRequest.SetChallengeResponses(challengeResponses);
				respondToAuthChallengeRequest.SetClientId(CLIENT_ID);

				auto respondToAuthChallengeOutcome = client.RespondToAuthChallenge(respondToAuthChallengeRequest);

				if (respondToAuthChallengeOutcome.IsSuccess()) {
					std::cout << "respondToAuthChallengeOutcome is Success"<< std::endl;

				}
				else {					
					std::cout << "respondToAuthChallengeOutcome is fail" <<  std::endl;
				}
					
			}
			else{
				std::cout << "initiateAuthOutcome is fail :" << initiateAuthOutcome.GetError().GetMessage() << std::endl;
			}
		}
    }

    Aws::ShutdownAPI(options);

    return 0;
}